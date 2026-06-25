#pragma once
class Vehicle { public: static bool IsAllowed(void* vehicle); static bool DriverSeatFree(void* vehicle); static bool PutPedInCarDirect(void* ped, void* vehicle, int seat); };
