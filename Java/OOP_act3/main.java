public class Main {
    public static void main(String []args) {
        String name = "Reindel Andrada";
        System.out.println(name);

        Vehicle myVehicle = new Vehicle("Honda", "Red", "CBR", "Manual", 2, 9.2, 2103, 10, 1000);

        myVehicle.warmUp();
        myVehicle.refuel(100);
        myVehicle.shiftGear(2);
        myVehicle.park();
    }
}
