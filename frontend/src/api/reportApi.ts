import api from "./axiosInstance";
import type { ApiResponse, ReportSummary } from "../types";

export interface ReportRow {
  status?: string;
  priority?: string;
  category?: string;
  engineer_name?: string;
  count: number;
  ticket_count?: number;
}

export async function getReportSummary() {
  const response = await api.get<ApiResponse<ReportSummary>>("/reports/summary");
  return response.data.data;
}

export async function getReportByStatus() {
  const response = await api.get<ApiResponse<ReportRow[]>>("/reports/status");
  return response.data.data;
}

export async function getReportByPriority() {
  const response = await api.get<ApiResponse<ReportRow[]>>("/reports/priority");
  return response.data.data;
}

export async function getReportByCategory() {
  const response = await api.get<ApiResponse<ReportRow[]>>("/reports/category");
  return response.data.data;
}

export async function getReportByEngineer() {
  const response = await api.get<ApiResponse<ReportRow[]>>("/reports/engineers");
  return response.data.data;
}
