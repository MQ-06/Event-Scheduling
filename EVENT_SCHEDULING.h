#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "tree.cpp"

class EventScheduling
{
public:
    Tree *root;

    EventScheduling() : root(nullptr) {}

    ~EventScheduling() { deleteTree(root); }

    void addEvent(Event *event)
    {
        string date = event->getTimeDate().substr(0, 10);
        int start = event->getStartTime();
        int end = event->getEndTime();

        vector<Event *> overlappingEvents = findOverlappingEvents(date, convertToTime(start), convertToTime(end));
        if (!overlappingEvents.empty())
        {
            throw invalid_argument("Cannot add event. Overlapping events are not allowed.");
        }

        root = addEventRec(root, event);
    }

    void updateEvent(int id, const string &newName, const string &newTimeDate, int newDuration)
    {
        Tree *node = findEventByID(root, id);
        if (!node)
        {
            throw invalid_argument("Event with the given ID does not exist.");
        }

        string newDate = newTimeDate.substr(0, 10);
        int newStartTime = convertToMinutes(newTimeDate.substr(11, 5));
        int newEndTime = newStartTime + newDuration;

        string newStartTimeStr = convertToTime(newStartTime);
        string newEndTimeStr = convertToTime(newEndTime);

        vector<Event *> overlappingEvents = findOverlappingEvents(newDate, newStartTimeStr, newEndTimeStr);

        vector<Event *> filtered;
        for (Event *event : overlappingEvents)
        {
            if (event->getEventID() != id)
            {
                filtered.push_back(event);
            }
        }

        if (!filtered.empty())
        {
            throw invalid_argument("Updated event overlaps with existing events.");
        }

        node->root_event->setEventName(newName);
        node->root_event->setTimeDate(newTimeDate);
        node->root_event->setEventDuration(newDuration);
    }

    void deleteEvent(int id)
    {
        try
        {
            root = deleteEventRec(root, id);
            cout << "Event with ID " << id << " successfully deleted." << endl;
        }
        catch (const invalid_argument &e)
        {
            cerr << e.what() << endl;
        }
    }

    vector<Event *> findOverlappingEvents(const string &date, const string &startTime, const string &endTime)
    {
        int start = convertToMinutes(startTime);
        int end = convertToMinutes(endTime);

        vector<Event *> overlaps;
        findOverlaps(root, date, start, end, overlaps);

        vector<Event *> actualOverlaps;
        for (Event *event : overlaps)
        {
            int eventStart = event->getStartTime();
            int eventEnd = event->getEndTime();

            if ((eventStart < end && eventEnd > start) ||
                (eventStart == end || eventEnd == start))
            {
                actualOverlaps.push_back(event);
            }
        }

        return actualOverlaps;
    }

    static bool compare(Event *a, Event *b)
    {
        return a->getStartTime() < b->getStartTime();
    }

    vector<pair<string, string>> findFreeTimeSlots(const string &date)
    {
        vector<Event *> events;
        collectEventsOnDate(root, date, events);

        sort(events.begin(), events.end(), compare);

        vector<pair<string, string>> freeSlots;
        int dayStart = 0;
        int dayEnd = 24 * 60;

        for (Event *event : events)
        {
            if (event->getStartTime() > dayStart)
            {
                freeSlots.emplace_back(
                    convertToTime(dayStart),
                    convertToTime(event->getStartTime()));
            }
            dayStart = max(dayStart, event->getEndTime());
        }

        if (dayStart < dayEnd)
        {
            freeSlots.emplace_back(convertToTime(dayStart), convertToTime(dayEnd));
        }

        return freeSlots;
    }

    void printFullSchedule()
    {
        printInOrder(root);
    }

private:
    Tree *addEventRec(Tree *node, Event *event)
    {
        if (!node)
            return new Tree(event);

        int eventStartTime = event->getStartTime();
        int eventEndTime = event->getEndTime();

        if (eventEndTime <= node->root_event->getStartTime())
        {
            node->lchild = addEventRec(node->lchild, event);
        }
        else if (eventStartTime >= node->root_event->getEndTime())
        {
            node->rchild = addEventRec(node->rchild, event);
        }
        else
        {
            throw invalid_argument("Overlapping events are not allowed.");
        }

        return node;
    }

    Tree *deleteEventRec(Tree *node, int id)
    {
        if (!node)
        {
            throw invalid_argument("Event with the given ID does not exist and cannot be deleted.");
        }

        if (node->root_event->getEventID() == id)
        {
            if (!node->lchild)
            {
                Tree *right = node->rchild;
                delete node;
                return right;
            }
            else if (!node->rchild)
            {
                Tree *left = node->lchild;
                delete node;
                return left;
            }
            else
            {
                Tree *minNode = findMin(node->rchild);
                node->root_event = minNode->root_event;
                node->rchild = deleteEventRec(node->rchild, minNode->root_event->getEventID());
            }
        }
        else if (id < node->root_event->getEventID())
        {
            node->lchild = deleteEventRec(node->lchild, id);
        }
        else
        {
            node->rchild = deleteEventRec(node->rchild, id);
        }

        return node;
    }

    Tree *findMin(Tree *node)
    {
        while (node && node->lchild)
        {
            node = node->lchild;
        }
        return node;
    }

    Tree *findEventByID(Tree *node, int id)
    {
        if (!node)
            return nullptr;
        if (node->root_event->getEventID() == id)
            return node;
        if (id < node->root_event->getEventID())
            return findEventByID(node->lchild, id);
        return findEventByID(node->rchild, id);
    }

    void findOverlaps(Tree *node, const string &startDate, int start, int end, vector<Event *> &overlaps)
    {
        if (!node)
            return;

        string eventDate = node->root_event->getTimeDate().substr(0, 10);
        if (eventDate == startDate)
        {
            if (node->root_event->getEndTime() > start && node->root_event->getStartTime() < end)
            {
                overlaps.push_back(node->root_event);
            }
        }

        if (node->lchild && node->lchild->root_event->getEndTime() > start)
        {
            findOverlaps(node->lchild, startDate, start, end, overlaps);
        }

        if (node->rchild && node->rchild->root_event->getStartTime() < end)
        {
            findOverlaps(node->rchild, startDate, start, end, overlaps);
        }
    }

    void collectEventsOnDate(Tree *node, const string &date, vector<Event *> &events)
    {
        if (!node)
            return;

        if (node->root_event->getTimeDate().substr(0, 10) == date)
        {
            events.push_back(node->root_event);
        }

        collectEventsOnDate(node->lchild, date, events);
        collectEventsOnDate(node->rchild, date, events);
    }

    void printInOrder(Tree *node)
    {
        if (!node)
            return;

        printInOrder(node->lchild);
        node->root_event->displayEvent();
        printInOrder(node->rchild);
    }

    void deleteTree(Tree *node)
    {
        if (!node)
            return;
        deleteTree(node->lchild);
        deleteTree(node->rchild);
        delete node;
    }

    int convertToMinutes(const string &time) const
    {
        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        return hour * 60 + minute;
    }

    string convertToTime(int minutes) const
    {
        int hour = minutes / 60;
        int minute = minutes % 60;
        return (hour < 10 ? "0" : "") + to_string(hour) + ":" + (minute < 10 ? "0" : "") + to_string(minute);
    }
};
