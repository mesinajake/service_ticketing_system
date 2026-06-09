import { BrowserRouter, Route, Routes } from "react-router-dom";

import { Navbar } from "./components/Navbar";
import { Sidebar } from "./components/Sidebar";
import { CreateTicket } from "./pages/CreateTicket";
import { Dashboard } from "./pages/Dashboard";
import { EditTicket } from "./pages/EditTicket";
import { Engineers } from "./pages/Engineers";
import { Reports } from "./pages/Reports";
import { TicketDetails } from "./pages/TicketDetails";
import { Tickets } from "./pages/Tickets";

export default function App() {
  return (
    <BrowserRouter>
      <div className="min-h-screen bg-zinc-100">
        <Navbar />
        <div className="mx-auto grid max-w-7xl gap-6 px-4 py-6 sm:px-6 lg:grid-cols-[220px_1fr] lg:px-8">
          <Sidebar />
          <main>
            <Routes>
              <Route path="/" element={<Dashboard />} />
              <Route path="/tickets" element={<Tickets />} />
              <Route path="/tickets/new" element={<CreateTicket />} />
              <Route path="/tickets/:id" element={<TicketDetails />} />
              <Route path="/tickets/:id/edit" element={<EditTicket />} />
              <Route path="/engineers" element={<Engineers />} />
              <Route path="/reports" element={<Reports />} />
            </Routes>
          </main>
        </div>
      </div>
    </BrowserRouter>
  );
}
