import { FormEvent, useEffect, useState } from "react";

import { createEngineer, getEngineers } from "../api/engineerApi";
import { ErrorMessage } from "../components/ErrorMessage";
import { LoadingSpinner } from "../components/LoadingSpinner";
import type { Engineer, EngineerInput } from "../types";

const emptyEngineer: EngineerInput = {
  full_name: "",
  email: "",
  role: "",
  department: ""
};

export function Engineers() {
  const [engineers, setEngineers] = useState<Engineer[]>([]);
  const [form, setForm] = useState<EngineerInput>(emptyEngineer);
  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(true);

  async function loadEngineers() {
    setEngineers(await getEngineers());
  }

  useEffect(() => {
    loadEngineers()
      .catch((loadError: Error) => setError(loadError.message))
      .finally(() => setIsLoading(false));
  }, []);

  async function handleSubmit(event: FormEvent) {
    event.preventDefault();
    setError("");
    try {
      await createEngineer(form);
      setForm(emptyEngineer);
      await loadEngineers();
    } catch (submitError) {
      setError(submitError instanceof Error ? submitError.message : "Unable to create engineer.");
    }
  }

  if (isLoading) {
    return <LoadingSpinner />;
  }

  return (
    <div className="grid gap-6">
      <div>
        <h1 className="text-2xl font-semibold text-zinc-950">Engineers</h1>
        <p className="mt-1 text-zinc-600">Manage support engineers and see active assigned ticket counts.</p>
      </div>

      <ErrorMessage message={error} />

      <div className="grid gap-6 lg:grid-cols-[1fr_380px]">
        <section className="grid gap-3">
          {engineers.map((engineer) => (
            <article key={engineer.id} className="rounded-lg border border-zinc-200 bg-white p-4">
              <div className="flex justify-between gap-3">
                <div>
                  <h2 className="font-semibold text-zinc-950">{engineer.full_name}</h2>
                  <p className="text-sm text-zinc-600">{engineer.email}</p>
                  <p className="mt-2 text-sm text-zinc-500">
                    {engineer.role} · {engineer.department}
                  </p>
                </div>
                <div className="text-right">
                  <p className="text-2xl font-semibold text-zinc-950">{engineer.active_ticket_count ?? 0}</p>
                  <p className="text-xs text-zinc-500">active tickets</p>
                </div>
              </div>
            </article>
          ))}
        </section>

        <form onSubmit={handleSubmit} className="grid h-fit gap-3 rounded-lg border border-zinc-200 bg-white p-4">
          <h2 className="text-lg font-semibold text-zinc-950">Add Engineer</h2>
          <input className="rounded-md border border-zinc-300 px-3 py-2" placeholder="Full name" value={form.full_name} onChange={(event) => setForm({ ...form, full_name: event.target.value })} required />
          <input className="rounded-md border border-zinc-300 px-3 py-2" placeholder="Email" type="email" value={form.email} onChange={(event) => setForm({ ...form, email: event.target.value })} required />
          <input className="rounded-md border border-zinc-300 px-3 py-2" placeholder="Role" value={form.role} onChange={(event) => setForm({ ...form, role: event.target.value })} required />
          <input className="rounded-md border border-zinc-300 px-3 py-2" placeholder="Department" value={form.department} onChange={(event) => setForm({ ...form, department: event.target.value })} required />
          <button className="rounded-md bg-blue-700 px-4 py-2 text-sm font-semibold text-white">Add Engineer</button>
        </form>
      </div>
    </div>
  );
}
