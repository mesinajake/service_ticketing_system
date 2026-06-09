import { useEffect, useState } from "react";

import {
  getReportByCategory,
  getReportByEngineer,
  getReportByPriority,
  getReportByStatus,
  type ReportRow
} from "../api/reportApi";
import { ErrorMessage } from "../components/ErrorMessage";
import { LoadingSpinner } from "../components/LoadingSpinner";

export function Reports() {
  const [statusRows, setStatusRows] = useState<ReportRow[]>([]);
  const [priorityRows, setPriorityRows] = useState<ReportRow[]>([]);
  const [categoryRows, setCategoryRows] = useState<ReportRow[]>([]);
  const [engineerRows, setEngineerRows] = useState<ReportRow[]>([]);
  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    Promise.all([getReportByStatus(), getReportByPriority(), getReportByCategory(), getReportByEngineer()])
      .then(([statusData, priorityData, categoryData, engineerData]) => {
        setStatusRows(statusData);
        setPriorityRows(priorityData);
        setCategoryRows(categoryData);
        setEngineerRows(engineerData);
      })
      .catch((loadError: Error) => setError(loadError.message))
      .finally(() => setIsLoading(false));
  }, []);

  if (isLoading) {
    return <LoadingSpinner />;
  }

  return (
    <div className="grid gap-6">
      <div>
        <h1 className="text-2xl font-semibold text-zinc-950">Reports</h1>
        <p className="mt-1 text-zinc-600">Visual summaries grouped by status, priority, category, and engineer.</p>
      </div>
      <ErrorMessage message={error} />
      <div className="grid gap-6 lg:grid-cols-2">
        <ReportPanel title="By Status" rows={statusRows} labelKey="status" />
        <ReportPanel title="By Priority" rows={priorityRows} labelKey="priority" />
        <ReportPanel title="By Category" rows={categoryRows} labelKey="category" />
        <ReportPanel title="By Engineer" rows={engineerRows} labelKey="engineer_name" />
      </div>
    </div>
  );
}

function ReportPanel({ title, rows, labelKey }: { title: string; rows: ReportRow[]; labelKey: keyof ReportRow }) {
  const max = Math.max(1, ...rows.map((row) => row.count));

  return (
    <section className="rounded-lg border border-zinc-200 bg-white p-4">
      <h2 className="mb-4 text-lg font-semibold text-zinc-950">{title}</h2>
      <div className="grid gap-3">
        {rows.map((row) => {
          const label = String(row[labelKey] ?? "Unknown");
          return (
            <div key={label}>
              <div className="mb-1 flex justify-between text-sm">
                <span className="font-medium text-zinc-700">{label}</span>
                <span className="text-zinc-500">{row.count}</span>
              </div>
              <div className="h-2 rounded-full bg-zinc-100">
                <div className="h-2 rounded-full bg-blue-600" style={{ width: `${(row.count / max) * 100}%` }} />
              </div>
            </div>
          );
        })}
      </div>
    </section>
  );
}
