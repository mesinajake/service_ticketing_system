import api from "./axiosInstance";
import type { ApiResponse, Engineer, EngineerInput } from "../types";

export async function getEngineers() {
  const response = await api.get<ApiResponse<Engineer[]>>("/engineers");
  return response.data.data;
}

export async function getEngineer(id: number) {
  const response = await api.get<ApiResponse<Engineer>>(`/engineers/${id}`);
  return response.data.data;
}

export async function createEngineer(engineer: EngineerInput) {
  const response = await api.post<ApiResponse<Engineer>>("/engineers", engineer);
  return response.data.data;
}
