import type { Status } from "../types";

interface StatusBadgeProps {
  status: Status;
}

const statusStyles: Record<Status, string> = {
  Open: "bg-blue-100 text-blue-800",
  "In Progress": "bg-amber-100 text-amber-800",
  Resolved: "bg-green-100 text-green-800",
  Closed: "bg-zinc-200 text-zinc-700"
};

export function StatusBadge({ status }: StatusBadgeProps) {
  return <span className={`rounded-full px-2.5 py-1 text-xs font-semibold ${statusStyles[status]}`}>{status}</span>;
}
