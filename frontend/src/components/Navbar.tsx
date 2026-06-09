import { Link } from "react-router-dom";

export function Navbar() {
  return (
    <header className="border-b border-zinc-200 bg-white">
      <div className="mx-auto flex max-w-7xl items-center justify-between px-4 py-4 sm:px-6 lg:px-8">
        <Link to="/" className="text-lg font-semibold text-zinc-950">
          Service Ticket System
        </Link>
        <span className="text-sm text-zinc-500">C++17 Crow API</span>
      </div>
    </header>
  );
}
