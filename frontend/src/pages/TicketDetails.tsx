import { useEffect, useState } from "react";
import { Link, useNavigate, useParams } from "react-router-dom";

import { getTicketLogs } from "../api/logApi";
import { deleteTicket, getTicket, updateTicketStatus } from "../api/ticketApi";
import { ErrorMessage } from "../components/ErrorMessage";
import { LoadingSpinner } from "../components/LoadingSpinner";
import { PriorityBadge } from "../components/PriorityBadge";
import { StatusBadge } from "../components/StatusBadge";
import type { ActivityLog, Status, Ticket } from "../types";
import { formatDate } from "../utils/format";

const statuses: Status[] = ["Open", "In Progress", "Resolved", "Closed"];

export function TicketDetails() {
  const { id } = useParams();
  const navigate = useNavigate();
  const [ticket, setTicket] = useState<Ticket | null>(null);
  const [logs, setLogs] = useState<ActivityLog[]>([]);
  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(true);

  async function loadData() {
    const ticketId = Number(id);
    const [ticketData, logData] = await Promise.all([getTicket(ticketId), getTicketLogs(ticketId)]);
    setTicket(ticketData);
    setLogs(logData);
  }

  useEffect(() => {
    loadData()
      .catch((loadError: Error) => setError(loadError.message))
      .finally(() => setIsLoading(false));
  }, [id]);

  async function handleStatusChange(status: Status) {
    await updateTicketStatus(Number(id), status);
    await loadData();
  }

  async function handleDelete() {
    if (!window.confirm("Delete this ticket?")) {
      return;
    }
    await deleteTicket(Number(id));
    navigate("/tickets");
  }

  if (isLoading) {
    return <LoadingSpinner />;
  }

  if (!ticket) {
    return <ErrorMessage message={error || "Ticket not found."} />;
  }

  return (
    <div className="grid gap-6">
      <div className="flex flex-col gap-3 sm:flex-row sm:items-start sm:justify-between">
        <div>
          <p className="text-sm text-zinc-500">Ticket #{ticket.id}</p>
          <h1 className="text-2xl font-semibold text-zinc-950">{ticket.title}</h1>
          <p className="mt-1 text-zinc-600">{ticket.description}</p>
        </div>
        <div className="flex gap-2">
          <Link className="rounded-md border border-zinc-300 px-4 py-2 text-sm font-semibold" to={`/tickets/${ticket.id}/edit`}>
            Edit
          </Link>
          <button className="rounded-md border border-red-200 px-4 py-2 text-sm font-semibold text-red-700" onClick={() => handleDelete().catch((deleteError: Error) => setError(deleteError.message))}>
            Delete
          </button>
        </div>
      </div>

      <ErrorMessage message={error} />

      <section className="grid gap-4 rounded-lg border border-zinc-200 bg-white p-4 md:grid-cols-2">
        <Detail label="Requester" value={`${ticket.requester_name} (${ticket.requester_email})`} />
        <Detail label="Assigned Engineer" value={ticket.assigned_engineer_name || "Unassigned"} />
        <Detail label="Category" value={ticket.category} />
        <div className="grid gap-1">
          <p className="text-sm font-medium text-zinc-500">Priority</p>
          <PriorityBadge priority={ticket.priority} />
        </div>
        <div className="grid gap-1">
          <p className="text-sm font-medium text-zinc-500">Status</p>
          <StatusBadge status={ticket.status} />
        </div>
        <Detail label="Resolution Notes" value={ticket.resolution_notes || "No resolution notes yet."} />
        <Detail label="Created" value={formatDate(ticket.created_at)} />
        <Detail label="Updated" value={formatDate(ticket.updated_at)} />
      </section>

      <section className="rounded-lg border border-zinc-200 bg-white p-4">
        <h2 className="mb-3 text-lg font-semibold text-zinc-950">Change Status</h2>
        <select
          className="rounded-md border border-zinc-300 px-3 py-2"
          value={ticket.status}
          onChange={(event) => handleStatusChange(event.target.value as Status).catch((statusError: Error) => setError(statusError.message))}
        >
          {statuses.map((status) => (
            <option key={status}>{status}</option>
          ))}
        </select>
      </section>

      <section className="rounded-lg border border-zinc-200 bg-white p-4">
        <h2 className="mb-4 text-lg font-semibold text-zinc-950">Activity Timeline</h2>
        <div className="grid gap-3">
          {logs.map((log) => (
            <div key={log.id} className="border-l-2 border-blue-600 pl-3">
              <p className="text-sm font-medium text-zinc-950">{log.description}</p>
              <p className="text-xs text-zinc-500">
                {log.action} · {formatDate(log.created_at)}
              </p>
            </div>
          ))}
        </div>
      </section>
    </div>
  );
}

function Detail({ label, value }: { label: string; value: string }) {
  return (
    <div>
      <p className="text-sm font-medium text-zinc-500">{label}</p>
      <p className="mt-1 text-zinc-900">{value}</p>
    </div>
  );
}
