import { useEffect, useState } from "react";
import { useNavigate, useParams } from "react-router-dom";

import { getEngineers } from "../api/engineerApi";
import { getTicket, updateTicket } from "../api/ticketApi";
import { ErrorMessage } from "../components/ErrorMessage";
import { LoadingSpinner } from "../components/LoadingSpinner";
import { TicketForm } from "../components/TicketForm";
import type { Engineer, Ticket, TicketInput } from "../types";

export function EditTicket() {
  const { id } = useParams();
  const navigate = useNavigate();
  const [ticket, setTicket] = useState<Ticket | null>(null);
  const [engineers, setEngineers] = useState<Engineer[]>([]);
  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    Promise.all([getTicket(Number(id)), getEngineers()])
      .then(([ticketData, engineerData]) => {
        setTicket(ticketData);
        setEngineers(engineerData);
      })
      .catch((loadError: Error) => setError(loadError.message))
      .finally(() => setIsLoading(false));
  }, [id]);

  async function handleSubmit(input: TicketInput) {
    await updateTicket(Number(id), input);
    navigate(`/tickets/${id}`);
  }

  if (isLoading) {
    return <LoadingSpinner />;
  }

  if (!ticket) {
    return <ErrorMessage message={error || "Ticket not found."} />;
  }

  return (
    <div className="grid gap-6">
      <div>
        <h1 className="text-2xl font-semibold text-zinc-950">Edit Ticket #{ticket.id}</h1>
        <p className="mt-1 text-zinc-600">Update ticket details, assignment, priority, and status.</p>
      </div>
      <ErrorMessage message={error} />
      <TicketForm engineers={engineers} initialValue={ticket} submitLabel="Save Changes" onSubmit={handleSubmit} />
    </div>
  );
}
