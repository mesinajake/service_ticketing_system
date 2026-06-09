import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import { deleteTicket, getTickets, type TicketFilters } from "../api/ticketApi";
import { ErrorMessage } from "../components/ErrorMessage";
import { LoadingSpinner } from "../components/LoadingSpinner";
import { TicketTable } from "../components/TicketTable";
import type { Category, Priority, Status, Ticket } from "../types";

const statuses: Status[] = ["Open", "In Progress", "Resolved", "Closed"];
const priorities: Priority[] = ["Low", "Medium", "High", "Critical"];
const categories: Category[] = [
  "Software Issue",
  "Hardware Issue",
  "Network Issue",
  "Account Access",
  "System Bug",
  "Client Request",
  "Others"
];

export function Tickets() {
  const [tickets, setTickets] = useState<Ticket[]>([]);
  const [filters, setFilters] = useState<TicketFilters>({});
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState("");

  async function loadTickets(nextFilters = filters) {
    setError("");
    const data = await getTickets(nextFilters);
    setTickets(data);
  }

  useEffect(() => {
    loadTickets()
      .catch((loadError: Error) => setError(loadError.message))
      .finally(() => setIsLoading(false));
  }, []);

  function updateFilter(key: keyof TicketFilters, value: string) {
    const nextFilters = { ...filters, [key]: value || undefined };
    setFilters(nextFilters);
    loadTickets(nextFilters).catch((loadError: Error) => setError(loadError.message));
  }

  async function handleDelete(id: number) {
    if (!window.confirm("Delete this ticket?")) {
      return;
    }
    await deleteTicket(id);
    await loadTickets();
  }

  return (
    <div className="grid gap-6">
      <div className="flex flex-col gap-3 sm:flex-row sm:items-center sm:justify-between">
        <div>
          <h1 className="text-2xl font-semibold text-zinc-950">Tickets</h1>
          <p className="mt-1 text-zinc-600">Search, filter, view, edit, and delete service tickets.</p>
        </div>
        <Link className="w-fit rounded-md bg-blue-700 px-4 py-2 text-sm font-semibold text-white" to="/tickets/new">
          New Ticket
        </Link>
      </div>

      <ErrorMessage message={error} />

      <section className="grid gap-3 rounded-lg border border-zinc-200 bg-white p-4 md:grid-cols-4">
        <input
          className="rounded-md border border-zinc-300 px-3 py-2"
          placeholder="Search ID, title, requester, engineer"
          onChange={(event) => updateFilter("search", event.target.value)}
        />
        <select className="rounded-md border border-zinc-300 px-3 py-2" onChange={(event) => updateFilter("status", event.target.value)}>
          <option value="">All Statuses</option>
          {statuses.map((status) => (
            <option key={status}>{status}</option>
          ))}
        </select>
        <select className="rounded-md border border-zinc-300 px-3 py-2" onChange={(event) => updateFilter("priority", event.target.value)}>
          <option value="">All Priorities</option>
          {priorities.map((priority) => (
            <option key={priority}>{priority}</option>
          ))}
        </select>
        <select className="rounded-md border border-zinc-300 px-3 py-2" onChange={(event) => updateFilter("category", event.target.value)}>
          <option value="">All Categories</option>
          {categories.map((category) => (
            <option key={category}>{category}</option>
          ))}
        </select>
      </section>

      {isLoading ? <LoadingSpinner /> : <TicketTable tickets={tickets} onDelete={(id) => handleDelete(id).catch((deleteError: Error) => setError(deleteError.message))} />}
    </div>
  );
}
