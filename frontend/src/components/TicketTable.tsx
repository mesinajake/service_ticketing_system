import { Link } from "react-router-dom";

import type { Ticket } from "../types";
import { formatDate } from "../utils/format";
import { PriorityBadge } from "./PriorityBadge";
import { StatusBadge } from "./StatusBadge";

interface TicketTableProps {
  tickets: Ticket[];
  onDelete: (id: number) => void;
}

export function TicketTable({ tickets, onDelete }: TicketTableProps) {
  return (
    <div className="overflow-x-auto rounded-lg border border-zinc-200 bg-white">
      <table className="w-full min-w-[980px] text-left text-sm">
        <thead className="border-b border-zinc-200 bg-zinc-50 text-xs uppercase text-zinc-500">
          <tr>
            <th className="px-4 py-3">ID</th>
            <th className="px-4 py-3">Title</th>
            <th className="px-4 py-3">Requester</th>
            <th className="px-4 py-3">Priority</th>
            <th className="px-4 py-3">Status</th>
            <th className="px-4 py-3">Category</th>
            <th className="px-4 py-3">Assigned To</th>
            <th className="px-4 py-3">Created</th>
            <th className="px-4 py-3 text-right">Actions</th>
          </tr>
        </thead>
        <tbody>
          {tickets.map((ticket) => (
            <tr key={ticket.id} className="border-b border-zinc-100">
              <td className="px-4 py-3 text-zinc-600">#{ticket.id}</td>
              <td className="px-4 py-3 font-medium text-zinc-950">{ticket.title}</td>
              <td className="px-4 py-3 text-zinc-600">{ticket.requester_name}</td>
              <td className="px-4 py-3">
                <PriorityBadge priority={ticket.priority} />
              </td>
              <td className="px-4 py-3">
                <StatusBadge status={ticket.status} />
              </td>
              <td className="px-4 py-3 text-zinc-600">{ticket.category}</td>
              <td className="px-4 py-3 text-zinc-600">{ticket.assigned_engineer_name || "Unassigned"}</td>
              <td className="px-4 py-3 text-zinc-600">{formatDate(ticket.created_at)}</td>
              <td className="px-4 py-3">
                <div className="flex justify-end gap-2">
                  <Link className="rounded-md border border-zinc-300 px-3 py-1.5 font-medium" to={`/tickets/${ticket.id}`}>
                    View
                  </Link>
                  <Link
                    className="rounded-md border border-zinc-300 px-3 py-1.5 font-medium"
                    to={`/tickets/${ticket.id}/edit`}
                  >
                    Edit
                  </Link>
                  <button
                    className="rounded-md border border-red-200 px-3 py-1.5 font-medium text-red-700"
                    onClick={() => onDelete(ticket.id)}
                  >
                    Delete
                  </button>
                </div>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
      {tickets.length === 0 && <p className="py-8 text-center text-sm text-zinc-500">No tickets found.</p>}
    </div>
  );
}
