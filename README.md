# Deadlock_DMA

This is an open-source DMA client for Valve's latest game, **Deadlock**. The project is written using **C++23**.

<img width="2010" height="1131" alt="Screenshot 2025-10-01 195604" src="https://github.com/user-attachments/assets/e897efc9-3ef1-4700-a833-00a9ac5b3c7c" />


## Build
- Open `Deadlock_DMA.sln` in **Visual Studio 2022**
- Download the latest [MemProcFS release](https://github.com/ufrisk/MemProcFS/releases/latest)
- Extract it into the `Dependencies/MemProcFS` folder
- Select configuration (`Release | x64` recommended)
- Build the solution (**Ctrl+Shift+B**)

## Runtime
When launching, place the following libraries from [MemProcFS](https://github.com/ufrisk/MemProcFS) next to the executable:
-  `FTD3XX.dll`
-  `FTD3XXWU.dll`
-  `leechore.dll`
-  `leechore_driver.dll`
-  `vmm.dll`
