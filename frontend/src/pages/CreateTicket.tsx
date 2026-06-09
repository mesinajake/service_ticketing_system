import { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";

import { getEngineers } from "../api/engineerApi";
import { createTicket } from "../api/ticketApi";
import { LoadingSpinner } from "../components/LoadingSpinner";
import { TicketForm } from "../components/TicketForm";
import type { Engineer, TicketInput } from "../types";

export function CreateTicket() {
  const [engineers, setEngineers] = useState<Engineer[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  const navigate = useNavigate();

  useEffect(() => {
    getEngineers()
      .then(setEngineers)
      .finally(() => setIsLoading(false));
  }, []);

  async function handleSubmit(ticket: TicketInput) {
    const created = await createTicket(ticket);
    navigate(`/tickets/${created.id}`);
  }

  if (isLoading) {
    return <LoadingSpinner />;
  }

  return (
    <div className="grid gap-6">
      <div>
        <h1 className="text-2xl font-semibold text-zinc-950">Create Ticket</h1>
        <p className="mt-1 text-zinc-600">Create a new service request with requester and assignment details.</p>
      </div>
      <TicketForm engineers={engineers} submitLabel="Create Ticket" onSubmit={handleSubmit} />
    </div>
  );
}
