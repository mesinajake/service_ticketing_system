export type Priority = "Low" | "Medium" | "High" | "Critical";
export type Status = "Open" | "In Progress" | "Resolved" | "Closed";
export type Category =
  | "Software Issue"
  | "Hardware Issue"
  | "Network Issue"
  | "Account Access"
  | "System Bug"
  | "Client Request"
  | "Others";

export interface Ticket {
  id: number;
  title: string;
  description: string;
  requester_name: string;
  requester_email: string;
  category: Category;
  priority: Priority;
  status: Status;
  assigned_engineer_id: number | null;
  assigned_engineer_name?: string | null;
  resolution_notes: string | null;
  created_at: string;
  updated_at: string;
}

export interface Engineer {
  id: number;
  full_name: string;
  email: string;
  role: string;
  department: string;
  active_ticket_count?: number;
  created_at: string;
}

export interface ActivityLog {
  id: number;
  ticket_id: number;
  action: string;
  description: string;
  created_at: string;
}

export interface ReportSummary {
  total: number;
  open: number;
  in_progress: number;
  resolved: number;
  closed: number;
  critical: number;
}

export interface ApiResponse<T> {
  success: boolean;
  data: T;
  error?: string;
}

export type TicketInput = Omit<Ticket, "id" | "assigned_engineer_name" | "created_at" | "updated_at">;
export type EngineerInput = Omit<Engineer, "id" | "active_ticket_count" | "created_at">;
