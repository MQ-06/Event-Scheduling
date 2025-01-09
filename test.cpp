#include <iostream>
#include "EVENT_SCHEDULING.h"

int main()
{
    EventScheduling scheduler;

    // Add events
    Event *event1 = new Event("Meeting with John", "2025-01-10 09:00", 60);
    Event *event2 = new Event("Doctor's Appointment", "2025-01-10 10:00", 30);
    Event *event3 = new Event("Lunch with Sarah", "2025-02-10 12:30", 90);
    Event *event4 = new Event("Conference Call", "2025-02-10 15:00", 45);

    scheduler.addEvent(event1);
    scheduler.addEvent(event2);
    scheduler.addEvent(event3);
    scheduler.addEvent(event4);

    // Print the full schedule
    std::cout << "Full schedule:" << std::endl;
    scheduler.printFullSchedule();

    // Update an event
    std::cout << "\nUpdating event 2 (Doctor's Appointment)..." << std::endl;
    // scheduler.updateEvent(event2->getEventID(), "Rescheduled Doctor's Appointment", "2025-01-10 14:00", 30);

    // Print updated schedule
    std::cout << "\nUpdated schedule:" << std::endl;
    scheduler.printFullSchedule();

    std::cout << "Overlapping events between 2025-01-10 09:00 and 2025-01-10 13:00:" << std::endl;
    std::vector<Event *> overlappingEvents = scheduler.findOverlappingEvents("2025-01-10", "09:00", "13:00");

    // std::cout << "\nOverlapping events between " << startTime << " and " << endTime << ":" << std::endl;
    for (Event *e : overlappingEvents)
    {
        e->displayEvent();
    }

    // Find free time slots for the day
    std::string date = "2025-01-10";
    std::vector<std::pair<std::string, std::string>> freeSlots = scheduler.findFreeTimeSlots(date);

    std::cout << "\nFree time slots for " << date << ":" << std::endl;
    for (const auto &slot : freeSlots)
    {
        std::cout << slot.first << " - " << slot.second << std::endl;
    }

    // Delete an event
    std::cout << "\nDeleting event 3 (Lunch with Sarah)..." << std::endl;
    scheduler.deleteEvent(event3->getEventID());

    // Print the final schedule
    std::cout << "\nFinal schedule after deletion:" << std::endl;
    scheduler.printFullSchedule();

    return 0;
}
