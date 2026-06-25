#pragma once
class Vehicle {
public:
    static void* FindTargetVehicle();
    static bool IsAllowed(void* vehicle);
};
