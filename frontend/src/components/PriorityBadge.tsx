import type { Priority } from "../types";

interface PriorityBadgeProps {
  priority: Priority;
}

const priorityStyles: Record<Priority, string> = {
  Low: "bg-green-100 text-green-800",
  Medium: "bg-blue-100 text-blue-800",
  High: "bg-orange-100 text-orange-800",
  Critical: "bg-red-100 text-red-800"
};

export function PriorityBadge({ priority }: PriorityBadgeProps) {
  return (
    <span className={`rounded-full px-2.5 py-1 text-xs font-semibold ${priorityStyles[priority]}`}>{priority}</span>
  );
}
