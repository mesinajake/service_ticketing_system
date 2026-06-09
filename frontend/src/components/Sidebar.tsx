import { NavLink } from "react-router-dom";

const links = [
  { href: "/", label: "Dashboard" },
  { href: "/tickets", label: "Tickets" },
  { href: "/tickets/new", label: "Create Ticket" },
  { href: "/engineers", label: "Engineers" },
  { href: "/reports", label: "Reports" }
];

export function Sidebar() {
  return (
    <aside className="rounded-lg border border-zinc-200 bg-white p-3">
      <nav className="grid gap-1">
        {links.map((link) => (
          <NavLink
            key={link.href}
            to={link.href}
            className={({ isActive }) =>
              `rounded-md px-3 py-2 text-sm font-medium ${
                isActive ? "bg-blue-50 text-blue-700" : "text-zinc-700 hover:bg-zinc-100"
              }`
            }
          >
            {link.label}
          </NavLink>
        ))}
      </nav>
    </aside>
  );
}
