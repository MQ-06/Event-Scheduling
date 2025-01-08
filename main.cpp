#include <iostream>
#include <string>
#include <vector>
#include "EVENT_SCHEDULING.h"
using namespace std;

int main()
{
    EventScheduling scheduler;
    int choice;

    do
    {
        cout << "\n================ Event Scheduling System ================\n";
        cout << "1. Add Event\n";
        cout << "2. Update Event\n";
        cout << "3. Delete Event\n";
        cout << "4. Find Overlapping Events\n";
        cout << "5. Find Free Time Slots\n";
        cout << "6. Print Full Schedule\n";
        cout << "7. Exit\n";
        cout << "========================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try
        {
            if (choice == 1)
            {
                string name, date;
                int duration;

                cout << "Enter Event Name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter Event Date & Time (YYYY-MM-DD HH:MM): ";
                getline(cin, date);

                cout << "Enter Event Duration (in minutes): ";
                cin >> duration;

                Event *newEvent = new Event(name, date, duration);
                scheduler.addEvent(newEvent);

                cout << "Event added successfully.\n";
            }
            else if (choice == 2)
            {
                int eventId;
                string newName, newTime;
                int newDuration;

                cout << "Enter Event ID to Update: ";
                cin >> eventId;

                cout << "Enter New Event Name: ";
                cin.ignore();
                getline(cin, newName);

                cout << "Enter New Event Date & Time (YYYY-MM-DD HH:MM): ";
                getline(cin, newTime);

                cout << "Enter New Event Duration (in minutes): ";
                cin >> newDuration;

                scheduler.updateEvent(eventId, newName, newTime, newDuration);

                cout << "Event updated successfully.\n";
            }
            else if (choice == 3)
            {
                int eventId;

                cout << "Enter Event ID to Delete: ";
                cin >> eventId;

                scheduler.deleteEvent(eventId);

                cout << "Event deleted successfully.\n";
            }
            else if (choice == 4)
            {
                int startHour, startMinute, endHour, endMinute;

                cout << "Enter Start Hour (24-hour format): ";
                cin >> startHour;

                cout << "Enter Start Minute: ";
                cin >> startMinute;

                cout << "Enter End Hour (24-hour format): ";
                cin >> endHour;

                cout << "Enter End Minute: ";
                cin >> endMinute;

                scheduler.getOverlappingEvents(startHour, startMinute, endHour, endMinute);
            }
            else if (choice == 5)
            {
                int year, month, day, startHour, endHour;

                cout << "Enter Year: ";
                cin >> year;

                cout << "Enter Month: ";
                cin >> month;

                cout << "Enter Day: ";
                cin >> day;

                cout << "Enter Start Hour (24-hour format): ";
                cin >> startHour;

                cout << "Enter End Hour (24-hour format): ";
                cin >> endHour;

                scheduler.findFreeTimeSlots(year, month, day, startHour, endHour);
            }
            else if (choice == 6)
            {
                cout << "Printing Full Schedule:\n";
                scheduler.printSchedule();
            }
            else if (choice != 7)
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const std::exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }

    } while (choice != 7);

    cout << "Exiting Event Scheduling System. Goodbye!\n";
    return 0;
}
