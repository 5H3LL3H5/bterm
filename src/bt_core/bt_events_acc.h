/*******************************************************************************
 *   Description:   Accumulator of Event-s
 *
 ******************************************************************************/

#ifndef _BT_EVENTS_ACC_H
#define _BT_EVENTS_ACC_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <deque>
#include <vector>
#include <memory>

#include <QObject>



/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

namespace BTCore {
    template<typename T>
        class EventsAcc;
}

/**
 * Events accumulator: it allows one to collect events and query events
 * starting from specified index.
 */
template<typename T>
class BTCore::EventsAcc
{
    /****************************************************************************
     * TYPES
     ***************************************************************************/

    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
public:

    /**
     * @param max_events_cnt
     *      Specify maximum count of events to be stored at the moment.
     *      When newly added events cause total number of events to exceed
     *      the maximum number, the oldest events will be removed.
     *
     *      Note that index of newly added events are constantly growing,
     *      independently of `max_events_cnt`.
     */
    explicit EventsAcc(size_t max_events_cnt) :
        max_events_cnt(max_events_cnt),
        events(),
        last_event_num(0)
    {

    }


    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    //-- maximum number of items that deque could contain
    size_t max_events_cnt;

    //-- actual collection of events
    std::deque<std::shared_ptr<T>> events;

    //-- incremented at every new event. never decremented.
    //   must always be >= events.size()
    unsigned long last_event_num;




    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/

    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    /**
     * Retrieve events from collection, beginning from specified index.
     *
     * @param start_event_num
     *      Event index starting from which events should be returned
     * @param p_last_event_num
     *      Pointer to the location where current last event index
     *      should be stored
     */
    std::vector<std::shared_ptr<T>> getEvents(
            unsigned long start_event_num,
            unsigned long *p_last_event_num
            )
    {
        std::vector<std::shared_ptr<T>> new_events {};

        if (start_event_num < this->last_event_num){
            //-- get number of the event at `this->events[0]`.
            //   Note: the condition (last_event_num >= events.size()) is always true.
            unsigned long idx0_event_num = this->last_event_num - this->events.size();

            //-- get start iterator
            typename std::deque<std::shared_ptr<T>>::iterator iter = events.cbegin();
            if (start_event_num > idx0_event_num){
                iter += (start_event_num - idx0_event_num);
            }

            //-- copy items to the vector that will be returned
            new_events.insert(
                    new_events.begin(),
                    iter, events.cend()
                    );
        }

        //-- if user asked to provide last event number, write it
        //   at user's location
        if (p_last_event_num != nullptr){
            *p_last_event_num = this->last_event_num;
        }

        return new_events;
    }


    /**
     * Add new event to collection
     */
    void addEvent(std::shared_ptr<T> p_event)
    {
        events.push_back(p_event);
        last_event_num++;

        //-- if we have too many events, delete oldest
        if (max_events_cnt > 0 && events.size() > max_events_cnt){
            int cnt_to_delete = events.size() - max_events_cnt;
            while (cnt_to_delete-- > 0){
                events.pop_front();
            }
        }
    }


};


#endif // _BT_EVENTS_ACC_H
