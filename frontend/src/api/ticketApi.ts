import api from "./axiosInstance";
import type { ApiResponse, Status, Ticket, TicketInput } from "../types";

export interface TicketFilters {
  search?: string;
  status?: string;
  priority?: string;
  category?: string;
}

export async function getTickets(filters: TicketFilters = {}, limit?: number) {
  const response = await api.get<ApiResponse<Ticket[]>>("/tickets", { params: { ...filters, limit } });
  return response.data.data;
}

export async function getTicket(id: number) {
  const response = await api.get<ApiResponse<Ticket>>(`/tickets/${id}`);
  return response.data.data;
}

export async function createTicket(ticket: TicketInput) {
  const response = await api.post<ApiResponse<Ticket>>("/tickets", ticket);
  return response.data.data;
}

export async function updateTicket(id: number, ticket: TicketInput) {
  const response = await api.put<ApiResponse<Ticket>>(`/tickets/${id}`, ticket);
  return response.data.data;
}

export async function deleteTicket(id: number) {
  await api.delete(`/tickets/${id}`);
}

export async function updateTicketStatus(id: number, status: Status) {
  const response = await api.patch<ApiResponse<Ticket>>(`/tickets/${id}/status`, { status });
  return response.data.data;
}

export async function assignTicket(id: number, engineerId: number) {
  const response = await api.patch<ApiResponse<Ticket>>(`/tickets/${id}/assign`, { engineer_id: engineerId });
  return response.data.data;
}
