import api from "./axiosInstance";
import type { ActivityLog, ApiResponse } from "../types";

export async function getLogs() {
  const response = await api.get<ApiResponse<ActivityLog[]>>("/logs");
  return response.data.data;
}

export async function getTicketLogs(ticketId: number) {
  const response = await api.get<ApiResponse<ActivityLog[]>>(`/tickets/${ticketId}/logs`);
  return response.data.data;
}
