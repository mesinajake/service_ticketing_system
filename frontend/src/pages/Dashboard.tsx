import { useEffect, useState } from "react";

import { getReportSummary } from "../api/reportApi";
import { getTickets } from "../api/ticketApi";
import { ErrorMessage } from "../components/ErrorMessage";
import { LoadingSpinner } from "../components/LoadingSpinner";
import { TicketCard } from "../components/TicketCard";
import type { ReportSummary, Ticket } from "../types";

const emptySummary: ReportSummary = {
  total: 0,
  open: 0,
  in_progress: 0,
  resolved: 0,
  closed: 0,
  critical: 0
};

export function Dashboard() {
  const [summary, setSummary] = useState<ReportSummary>(emptySummary);
  const [recentTickets, setRecentTickets] = useState<Ticket[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState("");

  useEffect(() => {
    Promise.all([getReportSummary(), getTickets({}, 5)])
      .then(([summaryData, ticketData]) => {
        setSummary(summaryData);
        setRecentTickets(ticketData);
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
        <h1 className="text-2xl font-semibold text-zinc-950">Dashboard</h1>
        <p className="mt-1 text-zinc-600">Service ticket counts and the five most recent tickets.</p>
      </div>

      <ErrorMessage message={error} />

      <section className="grid gap-4 sm:grid-cols-2 xl:grid-cols-6">
        {[
          ["Total", summary.total],
          ["Open", summary.open],
          ["In Progress", summary.in_progress],
          ["Resolved", summary.resolved],
          ["Closed", summary.closed],
          ["Critical", summary.critical]
        ].map(([label, value]) => (
          <div key={label} className="rounded-lg border border-zinc-200 bg-white p-4">
            <p className="text-sm font-medium text-zinc-500">{label}</p>
            <p className="mt-2 text-3xl font-semibold text-zinc-950">{value}</p>
          </div>
        ))}
      </section>

      <section className="grid gap-3">
        <h2 className="text-lg font-semibold text-zinc-950">Recent Tickets</h2>
        <div className="grid gap-3 lg:grid-cols-2">
          {recentTickets.map((ticket) => (
            <TicketCard key={ticket.id} ticket={ticket} />
          ))}
        </div>
      </section>
    </div>
  );
}
