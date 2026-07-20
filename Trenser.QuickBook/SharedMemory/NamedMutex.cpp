/*
 * File: NamedMutex.cpp
 * Description: Implements the NamedMutex class by providing operations to
 *              acquire and release ownership of a Windows named mutex and
 *              enabling synchronization of shared resources across multiple
 *              threads and processes.
 * Author: Trenser
 * Created: 12 June 2026
 */
#include "NamedMutex.h"

/*
* Function: NamedMutex::lock
* Description: Acquires ownership of the named mutex. If another thread
*              or process currently owns the mutex, the calling thread
*              blocks until the mutex becomes available.
* Parameters: None
* Returns: None
*/
void NamedMutex::lock()
{
	WaitForSingleObject(m_handle, INFINITE);
}

/*
* Function: NamedMutex::unlock
* Description: Releases ownership of the named mutex and allows another
*              waiting thread or process to acquire it.
* Parameters: None
* Returns: None
*/
void NamedMutex::unlock()
{
	ReleaseMutex(m_handle);
}
