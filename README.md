# Deadlock_DMA

This is an open-source DMA client for Valve's latest game, **Deadlock**. The project is written using **C++23**.

<img width="1920" height="1079" alt="Showcase image" src="https://raw.githubusercontent.com/CyN1ckal/Deadlock_DMA/refs/heads/master/Deadlock%201.1.png" />


## Build

- Open `Deadlock_DMA.sln` in **Visual Studio 2022**

- Select configuration (`Release | x64` recommended)

- Build the solution (**Ctrl+Shift+B**)

## Runtime

When launching, place the following libraries from [MemProcFS](https://github.com/ufrisk/MemProcFS) and [Makcu C++](https://github.com/K4HVH/makcu-cpp) next to the executable:
-  `FTD3XX.dll`
-  `FTD3XXWU.dll`
-  `leechore.dll`
-  `leechore_driver.dll`
-  `vmm.dll`
- `makcu-cpp.dll`
