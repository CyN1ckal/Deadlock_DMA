#pragma once

class DMA_Connection
{
public: /* Singleton interface */
	static DMA_Connection* GetInstance();

private:
	static inline DMA_Connection* m_Instance = nullptr;

public:
	VMM_HANDLE GetHandle();
	bool EndConnection();

private:
	VMM_HANDLE m_VMMHandle = nullptr;

private:
	DMA_Connection();
	~DMA_Connection();
};