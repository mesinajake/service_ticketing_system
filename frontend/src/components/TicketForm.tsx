import { FormEvent, useState } from "react";

import type { Category, Engineer, Priority, Status, TicketInput } from "../types";

const categories: Category[] = [
  "Software Issue",
  "Hardware Issue",
  "Network Issue",
  "Account Access",
  "System Bug",
  "Client Request",
  "Others"
];
const priorities: Priority[] = ["Low", "Medium", "High", "Critical"];
const statuses: Status[] = ["Open", "In Progress", "Resolved", "Closed"];

interface TicketFormProps {
  engineers: Engineer[];
  initialValue?: TicketInput;
  submitLabel: string;
  onSubmit: (ticket: TicketInput) => Promise<void>;
}

const emptyTicket: TicketInput = {
  title: "",
  description: "",
  requester_name: "",
  requester_email: "",
  category: "Software Issue",
  priority: "Medium",
  status: "Open",
  assigned_engineer_id: null,
  resolution_notes: null
};

export function TicketForm({ engineers, initialValue, submitLabel, onSubmit }: TicketFormProps) {
  const [form, setForm] = useState<TicketInput>(initialValue ?? emptyTicket);
  const [error, setError] = useState("");
  const [isSaving, setIsSaving] = useState(false);

  async function handleSubmit(event: FormEvent) {
    event.preventDefault();
    setError("");
    setIsSaving(true);

    try {
      await onSubmit(form);
    } catch (submitError) {
      setError(submitError instanceof Error ? submitError.message : "Unable to save ticket.");
    } finally {
      setIsSaving(false);
    }
  }

  return (
    <form onSubmit={handleSubmit} className="grid gap-4 rounded-lg border border-zinc-200 bg-white p-4">
      {error && <div className="rounded-md border border-red-200 bg-red-50 px-3 py-2 text-sm text-red-700">{error}</div>}

      <div className="grid gap-4 md:grid-cols-2">
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Title
          <input
            className="rounded-md border border-zinc-300 px-3 py-2"
            value={form.title}
            onChange={(event) => setForm({ ...form, title: event.target.value })}
            required
          />
        </label>
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Requester Name
          <input
            className="rounded-md border border-zinc-300 px-3 py-2"
            value={form.requester_name}
            onChange={(event) => setForm({ ...form, requester_name: event.target.value })}
            required
          />
        </label>
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Requester Email
          <input
            className="rounded-md border border-zinc-300 px-3 py-2"
            type="email"
            value={form.requester_email}
            onChange={(event) => setForm({ ...form, requester_email: event.target.value })}
            required
          />
        </label>
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Assigned Engineer
          <select
            className="rounded-md border border-zinc-300 px-3 py-2"
            value={form.assigned_engineer_id ?? ""}
            onChange={(event) =>
              setForm({ ...form, assigned_engineer_id: event.target.value ? Number(event.target.value) : null })
            }
          >
            <option value="">Unassigned</option>
            {engineers.map((engineer) => (
              <option key={engineer.id} value={engineer.id}>
                {engineer.full_name}
              </option>
            ))}
          </select>
        </label>
      </div>

      <label className="grid gap-1 text-sm font-medium text-zinc-700">
        Description
        <textarea
          className="min-h-28 rounded-md border border-zinc-300 px-3 py-2"
          value={form.description}
          onChange={(event) => setForm({ ...form, description: event.target.value })}
          required
        />
      </label>

      <div className="grid gap-4 md:grid-cols-3">
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Category
          <select
            className="rounded-md border border-zinc-300 px-3 py-2"
            value={form.category}
            onChange={(event) => setForm({ ...form, category: event.target.value as Category })}
          >
            {categories.map((category) => (
              <option key={category}>{category}</option>
            ))}
          </select>
        </label>
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Priority
          <select
            className="rounded-md border border-zinc-300 px-3 py-2"
            value={form.priority}
            onChange={(event) => setForm({ ...form, priority: event.target.value as Priority })}
          >
            {priorities.map((priority) => (
              <option key={priority}>{priority}</option>
            ))}
          </select>
        </label>
        <label className="grid gap-1 text-sm font-medium text-zinc-700">
          Status
          <select
            className="rounded-md border border-zinc-300 px-3 py-2"
            value={form.status}
            onChange={(event) => setForm({ ...form, status: event.target.value as Status })}
          >
            {statuses.map((status) => (
              <option key={status}>{status}</option>
            ))}
          </select>
        </label>
      </div>

      <label className="grid gap-1 text-sm font-medium text-zinc-700">
        Resolution Notes
        <textarea
          className="min-h-20 rounded-md border border-zinc-300 px-3 py-2"
          value={form.resolution_notes ?? ""}
          onChange={(event) => setForm({ ...form, resolution_notes: event.target.value || null })}
        />
      </label>

      <button
        className="w-fit rounded-md bg-blue-700 px-4 py-2 text-sm font-semibold text-white hover:bg-blue-800 disabled:opacity-60"
        disabled={isSaving}
      >
        {isSaving ? "Saving..." : submitLabel}
      </button>
    </form>
  );
}
