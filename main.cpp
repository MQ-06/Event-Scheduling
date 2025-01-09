#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "EVENT_SCHEDULING.h"
using namespace std;

void menu()
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
}

int main()
{
    EventScheduling scheduler;
    int choice;

    do
    {
        menu();

        while (!(cin >> choice) || choice < 1 || choice > 7)
        {
            cout << "Invalid choice. Please enter a number between 1 and 7: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

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
                string date, startTime, endTime;

                cout << "Enter Date (YYYY-MM-DD): ";
                cin >> date;

                cout << "Enter Start Time (HH:MM): ";
                cin >> startTime;

                cout << "Enter End Time (HH:MM): ";
                cin >> endTime;

                vector<Event *> overlappingEvents = scheduler.findOverlappingEvents(date, startTime, endTime);
                if (overlappingEvents.empty())
                {
                    cout << "No overlapping events found.\n";
                }
                else
                {
                    cout << "Overlapping Events:\n";
                    for (Event *event : overlappingEvents)
                    {
                        event->displayEvent();
                    }
                }
            }
            else if (choice == 5)
            {
                string date;

                cout << "Enter Date (YYYY-MM-DD): ";
                cin >> date;

                vector<pair<string, string>> freeSlots = scheduler.findFreeTimeSlots(date);
                if (freeSlots.empty())
                {
                    cout << "No free time slots available.\n";
                }
                else
                {
                    cout << "Free Time Slots:\n";
                    for (const auto &slot : freeSlots)
                    {
                        cout << slot.first << " - " << slot.second << "\n";
                    }
                }
            }
            else if (choice == 6)
            {
                cout << "Printing Full Schedule:\n";
                scheduler.printFullSchedule();
            }
            else if (choice != 7)
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Invalid Argument Error: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 7);

    cout << "Goodbye :) \n";
    return 0;
}
