#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "tree.cpp"

class EventScheduling
{
private:
    Tree *root;

    // Helper: Compare two events based on start date and time
    int compareEvents(const Event *e1, const Event *e2)
    {
        return e1->getTimeDate().compare(e2->getTimeDate());
    }

    // Helper: Add event to the tree recursively
    Tree *addEvent(Tree *node, Event *event)
    {
        if (!node)
        {
            return new Tree(event);
        }

        if (hasOverlap(node, *event))
        {
            throw std::runtime_error("Event overlaps with an existing event.");
        }

        int cmp = compareEvents(event, node->root_event);
        if (cmp < 0)
        {
            node->lchild = addEvent(node->lchild, event);
        }
        else if (cmp > 0)
        {
            node->rchild = addEvent(node->rchild, event);
        }
        else
        {
            throw std::runtime_error("Event with the same start time already exists.");
        }

        return node;
    }

    // Helper: Find event by ID recursively
    Tree *findEvent(Tree *node, int eventId)
    {
        if (!node)
            return nullptr;
        if (node->root_event->getEventID() == eventId)
            return node;

        Tree *leftSearch = findEvent(node->lchild, eventId);
        if (leftSearch)
            return leftSearch;

        return findEvent(node->rchild, eventId);
    }

    // Helper: Delete event and rearrange the tree
    Tree *deleteEvent(Tree *node, int eventId)
    {
        if (!node)
            return nullptr;

        if (node->root_event->getEventID() == eventId)
        {
            if (!node->lchild)
            {
                Tree *temp = node->rchild;
                delete node;
                return temp;
            }
            else if (!node->rchild)
            {
                Tree *temp = node->lchild;
                delete node;
                return temp;
            }

            Tree *minNode = findMin(node->rchild);
            node->root_event = minNode->root_event;
            node->rchild = deleteEvent(node->rchild, minNode->root_event->getEventID());
        }
        else if (eventId < node->root_event->getEventID())
        {
            node->lchild = deleteEvent(node->lchild, eventId);
        }
        else
        {
            node->rchild = deleteEvent(node->rchild, eventId);
        }

        return node;
    }

    // Helper: Find the minimum node in the subtree
    Tree *findMin(Tree *node)
    {
        while (node && node->lchild)
            node = node->lchild;
        return node;
    }

    // Helper: Check for overlapping events
    bool hasOverlap(Tree *node, const Event &event)
    {
        if (!node)
            return false;

        // Get the start and end times of the existing event
        int startYear, startMonth, startDay, startHour, startMinute;
        node->root_event->extract_components(startYear, startMonth, startDay, startHour, startMinute);

        int endYear = startYear, endMonth = startMonth, endDay = startDay, endHour = startHour, endMinute = startMinute;
        node->root_event->calculateEndTime(endYear, endMonth, endDay, endHour, endMinute);

        // Get the start and end times of the new event
        int newStartYear, newStartMonth, newStartDay, newStartHour, newStartMinute;
        event.extract_components(newStartYear, newStartMonth, newStartDay, newStartHour, newStartMinute);

        int newEndYear = newStartYear, newEndMonth = newStartMonth, newEndDay = newStartDay, newEndHour = newStartHour, newEndMinute = newStartMinute;
        event.calculateEndTime(newEndYear, newEndMonth, newEndDay, newEndHour, newEndMinute);

        // Check for overlap
        bool overlap = !(newEndYear < startYear ||
                         (newEndYear == startYear && newEndMonth < startMonth) ||
                         (newEndYear == startYear && newEndMonth == startMonth && newEndDay < startDay) ||
                         (newEndYear == startYear && newEndMonth == startMonth && newEndDay == startDay &&
                          (newEndHour < startHour || (newEndHour == startHour && newEndMinute <= startMinute))) ||
                         (endYear < newStartYear ||
                          (endYear == newStartYear && endMonth < newStartMonth) ||
                          (endYear == newStartYear && endMonth == newStartMonth && endDay < newStartDay) ||
                          (endYear == newStartYear && endMonth == newStartMonth && endDay == newStartDay &&
                           (endHour < newStartHour || (endHour == newStartHour && endMinute <= newStartMinute)))));

        return overlap || hasOverlap(node->lchild, event) || hasOverlap(node->rchild, event);
    }

    // Helper: Print events in-order
    void printInOrder(Tree *node)
    {
        if (!node)
            return;

        printInOrder(node->lchild);
        node->root_event->displayEvent();
        std::cout << std::endl;
        printInOrder(node->rchild);
    }

    // Helper: Find overlapping events for a specific time range
    void getOverlappingEvents(Tree *node, int startHour, int startMinute, int endHour, int endMinute, std::vector<Event *> &overlappingEvents)
    {
        if (!node)
            return;

        int eventStartYear, eventStartMonth, eventStartDay, eventStartHour, eventStartMinute;
        node->root_event->extract_components(eventStartYear, eventStartMonth, eventStartDay, eventStartHour, eventStartMinute);

        int eventEndYear = eventStartYear, eventEndMonth = eventStartMonth, eventEndDay = eventStartDay, eventEndHour = eventStartHour, eventEndMinute = eventStartMinute;
        node->root_event->calculateEndTime(eventEndYear, eventEndMonth, eventEndDay, eventEndHour, eventEndMinute);

        int eventStartTime = eventStartHour * 60 + eventStartMinute;
        int eventEndTime = eventEndHour * 60 + eventEndMinute;

        // Check if the event overlaps with the given range
        if ((eventStartTime < endHour * 60 + endMinute) && (eventEndTime > startHour * 60 + startMinute))
        {
            overlappingEvents.push_back(node->root_event);
        }

        getOverlappingEvents(node->lchild, startHour, startMinute, endHour, endMinute, overlappingEvents);
        getOverlappingEvents(node->rchild, startHour, startMinute, endHour, endMinute, overlappingEvents);
    }

public:
    EventScheduling() : root(nullptr) {}

    void addEvent(Event *event)
    {
        root = addEvent(root, event);
    }

    void updateEvent(int eventId, const std::string &newName, const std::string &newTime, int newDuration)
    {
        Tree *node = findEvent(root, eventId);
        if (!node)
            throw std::runtime_error("Event not found.");

        Event updatedEvent(newName, newTime, newDuration);
        deleteEvent(eventId);    // Temporarily remove the event
        addEvent(&updatedEvent); // Try to re-add the updated event
    }

    void deleteEvent(int eventId)
    {
        root = deleteEvent(root, eventId);
    }

    void printSchedule()
    {
        printInOrder(root);
    }

    std::vector<Event *> getEventsForDay(int year, int month, int day)
    {
        std::vector<Event *> events;
        getEventsForDay(root, year, month, day, events);
        return events;
    }

    void getEventsForDay(Tree *node, int year, int month, int day, std::vector<Event *> &events)
    {
        if (!node)
            return;

        int eventYear, eventMonth, eventDay, eventHour, eventMinute;
        node->root_event->extract_components(eventYear, eventMonth, eventDay, eventHour, eventMinute);

        // Check if the event falls on the specified day
        if (eventYear == year && eventMonth == month && eventDay == day)
        {
            events.push_back(node->root_event);
        }

        getEventsForDay(node->lchild, year, month, day, events);
        getEventsForDay(node->rchild, year, month, day, events);
    }

    void findFreeTimeSlots(int year, int month, int day, int startHour, int endHour)
    {
        std::vector<Event *> events = getEventsForDay(year, month, day);

        // Sort events by start time
        sort(events.begin(), events.end(), [](Event *e1, Event *e2)
             { return e1->getTimeDate() < e2->getTimeDate(); });

        std::cout << "Free time slots for " << year << "-" << month << "-" << day << ":" << std::endl;

        // Check for gaps between events
        int lastEndTime = startHour * 60; // Start of the day in minutes

        for (Event *event : events)
        {
            int eventStartYear, eventStartMonth, eventStartDay, eventStartHour, eventStartMinute;
            event->extract_components(eventStartYear, eventStartMonth, eventStartDay, eventStartHour, eventStartMinute);
            int eventEndYear = eventStartYear, eventEndMonth = eventStartMonth, eventEndDay = eventStartDay, eventEndHour = eventStartHour, eventEndMinute = eventStartMinute;
            event->calculateEndTime(eventEndYear, eventEndMonth, eventEndDay, eventEndHour, eventEndMinute);

            int eventStartTime = eventStartHour * 60 + eventStartMinute;
            int eventEndTime = eventEndHour * 60 + eventEndMinute;

            // Check for free time before the event
            if (eventStartTime > lastEndTime)
            {
                std::cout << "Free time: " << lastEndTime / 60 << ":00 - " << eventStartTime / 60 << ":00" << std::endl;
            }

            lastEndTime = eventEndTime;
        }

        // Check for free time after the last event
        if (lastEndTime < endHour * 60)
        {
            std::cout << "Free time: " << lastEndTime / 60 << ":00 - " << endHour << ":00" << std::endl;
        }
    }

    // Function to get overlapping events for a specific time range
    void getOverlappingEvents(int startHour, int startMinute, int endHour, int endMinute)
    {
        std::vector<Event *> overlappingEvents;
        getOverlappingEvents(root, startHour, startMinute, endHour, endMinute, overlappingEvents);

        std::cout << "Overlapping events from " << startHour << ":" << startMinute
                  << " to " << endHour << ":" << endMinute << " are:" << std::endl;

        for (Event *event : overlappingEvents)
        {
            event->displayEvent();
        }
    }
};

