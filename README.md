
# Deadlock_DMA
  
This is an open-source DMA client for Valve's latest game, **Deadlock**. The project is written using **C++23**.

## Build

- Open `Deadlock_DMA.sln` in **Visual Studio 2022**

- Select configuration (`Release | x64` recommended)

- Build the solution (**Ctrl+Shift+B**)

## Runtime

When launching, place the following libraries from [MemProcFS](https://github.com/ufrisk/MemProcFS) next to the executable:
-  `FTD3XX.dll`
-  `FTD3XXWU.dll`
-  `leechore.dll`
-  `leechore_driver.dll`
-  `vmm.dll`