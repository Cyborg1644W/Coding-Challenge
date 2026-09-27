//brand
//model
//engineDisplacement
//color
//fuelLevel
//fuelCapacity
//maxSpeed
//mileage
//enginetemperature
//transmissionType


public class Vehicle {
    //Attributes
    private String brand;
    private String color;
    private String model;
    private String transmissionType;
    private double fuelLevel;
    private double fuelCapacity;
    private double mileage;
    private double engineTemperature;
    private int engineDisplacement ;


    private int currentGear = 0;
    private int currentSpeed = 0;
    private boolean isKickStandDown = true;
    private boolean isEngineRunning = false;

    //constructor
    public Vehicle (String brand, String color, String model,String transmissionType, double fuelLevel, double fuelCapacity, double mileage, double engineTemperature, int engineDisplacement) {
        this.brand = brand;
        this.color = color;
        this.model = model;
        this.transmissionType = transmissionType;
        this.fuelLevel = fuelLevel;
        this.fuelCapacity = fuelCapacity;
        this.mileage = mileage;
        this.engineTemperature = engineTemperature;
        this.engineDisplacement = engineDisplacement;
    }

    //methods

    public static void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    public void shiftGear(int chosenGear) {
        if("CVT".equalsIgnoreCase(transmissionType)) {
            System.out.println("CVT transmission detected, cannot shift gears");
            return;
        } if (chosenGear > 6 || chosenGear < 0) {
            System.out.println("Invalid gear, please choose between 1-6");
            return;
        }
        currentGear = chosenGear;
    }

    public void toggleEngine() {
        if(isEngineRunning) {
            isEngineRunning = false;
            System.out.println("Engine switched off.");
        } else {
            if (fuelLevel <= 0) {
                System.out.println("Cannot turn the engine, Fuel too low.");
                return;
            } else {
                isEngineRunning = true;
                System.out.println("Engine Started.");
            }
        }
    }

    public void refuel (int addedFuel) {
        if(fuelLevel < fuelCapacity) {
            fuelLevel += addedFuel;
            if(fuelLevel > fuelCapacity) {
                fuelLevel = fuelCapacity;
            }

            System.out.println("Refueled Successfully, Current Fuel: " + fuelLevel);
        } else {
            System.out.println("Vehicle is Already Full.");
        }
    }

    public void warmUp() {
        if(!isEngineRunning) {
            System.out.println("Engine is off, attempting to start before warming up.");
            toggleEngine();

            if(!isEngineRunning) {
                System.out.println("Cannot warmup, the engine failed to start.");
                return;
            }
        }
        System.out.println("Engine idling..");
        while (engineTemperature < 33) {
            engineTemperature += 0.0001;
            clearScreen();
            System.out.println("Current Temperature: " + engineTemperature);
        }
            System.out.println("Engine has reached normal operating temperature.");
    }

    public void park() {
        if (currentSpeed > 0) {
            System.out.println("Cannot park if the vehicle is moving.");
            return;
        }

        isKickStandDown = true;
        System.out.println("Kickstand lowered.");
        if(isEngineRunning) {
            isEngineRunning = false;
            System.out.println("Engine switched off.");
        }
    }
}
