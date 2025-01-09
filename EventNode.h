#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace std;

class Event
{
public:
    int event_id;
    string event_name;
    string time_date;
    int event_duration;
    static vector<int> unique_id;

    Event(string name, string date, int duration)
        : event_name(name), event_duration(duration)
    {
        setTimeDate(date);
        event_id = generateUniqueID();
    }

    int getEventID() const { return event_id; }
    string getEventName() const { return event_name; }
    string getTimeDate() const { return time_date; }
    int getEventDuration() const { return event_duration; }

    void setEventName(string name)
    {
        if (name.empty())
        {
            throw invalid_argument("Event name cannot be empty.");
        }
        event_name = name;
    }

    void setTimeDate(string date)
    {
        if (!isValidTimeDate(date))
        {
            throw invalid_argument("Invalid time_date format. Use 'YYYY-MM-DD HH:MM'.");
        }
        time_date = date;
    }

    void setEventDuration(int duration)
    {
        if (duration <= 0)
        {
            throw invalid_argument("Event duration must be positive.");
        }
        event_duration = duration;
    }

    void displayEvent() const
    {
        cout << "Event ID: " << event_id << endl;
        cout << "Event Name: " << event_name << endl;
        cout << "Event Date & Time: " << time_date << endl;
        cout << "Event Duration: " << event_duration << " minutes" << endl;
    }

    bool isValidTimeDate(const string &date) const
    {
        if (date.length() != 16)
            return false;

        if (date[4] != '-' || date[7] != '-' || date[10] != ' ' || date[13] != ':')
            return false;

        for (int i = 0; i < date.length(); i++)
        {
            if (i == 4 || i == 7 || i == 10 || i == 13)
                continue;
            if (!isdigit(date[i]))
                return false;
        }

        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        int hour = stoi(date.substr(11, 2));
        int minute = stoi(date.substr(14, 2));

        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > getDaysInMonth(year, month))
            return false;
        if (hour < 0 || hour > 23)
            return false;
        if (minute < 0 || minute > 59)
            return false;

        return true;
    }

    int generateUniqueID()
    {
        int new_id = rand() % 10000 + 1;
        while (!isUniqueID(new_id))
        {
            new_id = rand() % 10000 + 1;
        }
        unique_id.push_back(new_id);
        return new_id;
    }

    bool isUniqueID(int id) const
    {
        for (int existing_id : unique_id)
        {
            if (existing_id == id)
            {
                return false;
            }
        }
        return true;
    }

    static vector<int> getUniqueIDs()
    {
        return unique_id;
    }

    void extract_components(const string &time_date, int &year, int &month, int &day, int &hour, int &minute) const
    {
        year = stoi(time_date.substr(0, 4));
        month = stoi(time_date.substr(5, 2));
        day = stoi(time_date.substr(8, 2));
        hour = stoi(time_date.substr(11, 2));
        minute = stoi(time_date.substr(14, 2));
    }

    void calculateEndTime(int &year, int &month, int &day, int &hour, int &minute, int duration) const
    {
        minute += duration;

        while (minute >= 60)
        {
            minute -= 60;
            hour++;
        }

        while (hour >= 24)
        {
            hour -= 24;
            day++;
        }

        int daysInMonth = getDaysInMonth(year, month);
        while (day > daysInMonth)
        {
            day -= daysInMonth;
            month++;

            if (month > 12)
            {
                month = 1;
                year++;
            }

            daysInMonth = getDaysInMonth(year, month);
        }
    }

    int getStartTime() const
    {
        int year, month, day, hour, minute;
        extract_components(time_date, year, month, day, hour, minute);
        return hour * 60 + minute;
    }

    int getEndTime() const
    {
        int year, month, day, hour, minute;
        extract_components(time_date, year, month, day, hour, minute);
        calculateEndTime(year, month, day, hour, minute, event_duration);
        return hour * 60 + minute;
    }

    int getDaysInMonth(int year, int month) const
    {
        if (month == 4 || month == 6 || month == 9 || month == 11)
            return 30;
        else if (month == 2)
            return isLeapYear(year) ? 29 : 28;
        else
            return 31;
    }

    bool isLeapYear(int year) const
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return true;
        return false;
    }
};

vector<int> Event::unique_id;
