/*
 * File: Factory.h
 * Description: Provides a generic factory utility to create objects.
 * Author: Trenser
 * Created: 09-May-2026
 */

#pragma once
#include <utility>

class Factory
{
public:

    /*
     * Function: getObject
     * Description: Creates and returns a dynamically allocated object of type T.
     * Parameters:
     *    T - the type of object to be created
     *    Args - constructor arguments forwarded to T's constructor
     * Returns:
     *    T* - pointer to the newly created object
     */
    template<typename T, typename... Args>
    static T* getObject(Args&&... args)
    {
        return new T(std::forward<Args>(args)...);
    }
};