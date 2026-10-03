#pragma once

class DMA_Connection
{
public: /* Singleton interface */
	static DMA_Connection* GetInstance();

private:
	static inline DMA_Connection* m_Instance = nullptr;
	static inline std::mutex m_InstanceMutex{};

public:
	VMM_HANDLE GetHandle() const;
	bool EndConnection();
	bool RestartConnection();

private:
	VMM_HANDLE m_VMMHandle = nullptr;

private:
	DMA_Connection();
	~DMA_Connection();
};
