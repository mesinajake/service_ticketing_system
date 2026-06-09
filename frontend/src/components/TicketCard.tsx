import { Link } from "react-router-dom";

import type { Ticket } from "../types";
import { formatDate } from "../utils/format";
import { PriorityBadge } from "./PriorityBadge";
import { StatusBadge } from "./StatusBadge";

interface TicketCardProps {
  ticket: Ticket;
}

export function TicketCard({ ticket }: TicketCardProps) {
  return (
    <Link to={`/tickets/${ticket.id}`} className="block rounded-lg border border-zinc-200 bg-white p-4 hover:bg-zinc-50">
      <div className="flex items-start justify-between gap-3">
        <div>
          <p className="text-sm text-zinc-500">#{ticket.id}</p>
          <h3 className="font-semibold text-zinc-950">{ticket.title}</h3>
          <p className="mt-1 text-sm text-zinc-600">{ticket.requester_name}</p>
        </div>
        <div className="flex flex-col items-end gap-2">
          <PriorityBadge priority={ticket.priority} />
          <StatusBadge status={ticket.status} />
        </div>
      </div>
      <p className="mt-3 text-xs text-zinc-500">Created {formatDate(ticket.created_at)}</p>
    </Link>
  );
}
