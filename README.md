# SystemC Basics Tutorial

Welcome to the SystemC Basics repository! This repository contains 30 distinct examples covering various concepts of SystemC, progressively organized from simple boolean logic to advanced concurrency and custom channel implementations.

## Categories

### A. Basic Combinational Logic (`A_Combinational/`)
- `01_HalfAdder.cpp`: Simple boolean logic.
- `02_FullAdder.cpp`: Module instantiation.
- `03_Multiplexer.cpp`: Using `if/else`.
- `04_Decoder.cpp`: Using `switch`.
- `05_ALU_Basic.cpp`: Basic math operations.

### B. Sequential Logic (`B_Sequential/`)
- `06_DFlipFlop.cpp`: Clock edges and basic memory.
- `07_TFlipFlop.cpp`: Asynchronous reset.
- `08_ShiftRegister.cpp`: Serial to Parallel conversion.
- `09_Counter.cpp`: Up/Down counters.
- `10_LFSR.cpp`: Feedback loops.

### C. Data Types (`C_DataTypes/`)
- `11_FixedPoint.cpp`: `sc_fixed` math.
- `12_LogicValues.cpp`: `sc_logic` ('X', 'Z' states).
- `13_BitVectors.cpp`: `sc_bv` concatenation.
- `14_TypeCasting.cpp`: standard C++ to SystemC type conversions.
- `15_MemoryArray.cpp`: SRAM modeling using arrays.

### D. Processes (`D_Processes/`)
- `16_Method_vs_Thread.cpp`: `SC_METHOD` vs `SC_THREAD`.
- `17_CTHREAD.cpp`: Cycle-accurate modeling.
- `18_MultipleClocks.cpp`: Handling different clock domains.
- `19_DynamicSensitivity.cpp`: `next_trigger()`.
- `20_ProcessControl.cpp`: Suspending and resuming processes.

### E. Events (`E_Events/`)
- `21_SimpleEvent.cpp`: Basic `sc_event`.
- `22_DelayedNotification.cpp`: `notify(time)`.
- `23_EventLists.cpp`: Waiting for multiple events.
- `24_Timeouts.cpp`: Wait with timeout.
- `25_Mutex.cpp`: Mutual exclusion using `sc_mutex`.

### F. Channels (`F_Channels/`)
- `26_Semaphore.cpp`: Resource counting with `sc_semaphore`.
- `27_FIFO.cpp`: Bounded buffers with `sc_fifo`.
- `28_SignalResolved.cpp`: Multiple drivers on one signal.
- `29_CustomChannel.cpp`: Creating custom interfaces and channels.
- `30_PortArrays.cpp`: Arrays of ports for scalable design.

### G. Advanced Architecture (`G_Architecture/`)
- `31_DirectMappedCache.cpp`: Basic direct-mapped cache memory modeling.
- `32_SetAssociativeCache.cpp`: 2-Way Set Associative Cache with LRU replacement.
- `33_BasicPipeline.cpp`: 3-stage Instruction Pipeline simulating Fetch, Decode, Execute.
- `34_BusArbiter.cpp`: Round-Robin Bus Arbiter managing multiple requests.
- `35_TLM_Introduction.cpp`: Conceptual introduction to Transaction Level Modeling (TLM).
- `36_UART_Transmitter.cpp`: UART transmitter modeling serial communication protocols.

## Compilation

You can compile any of these files individually using `g++`. For example, to compile `01_HalfAdder.cpp`:

```bash
g++ A_Combinational/01_HalfAdder.cpp -o HalfAdder.exe -I C:\msys64\mingw64\include -L C:\msys64\mingw64\lib -lsystemc
.\HalfAdder.exe
```
