#include "../common.h"
#include "processes.h"

#include "../system/log.h"

namespace pegas
{
		/************************************************************************
		    Process class implementation
		*************************************************************************/
		
		Process::Process(): m_handle(0), m_owner(0)  
		{
			m_currentStatus = k_processStatusNotStarted;
		}

		void Process::_start(ProcessHandle myHandle, ProcessManagerPtr owner)
		{
			start(myHandle, owner);
		}

				
		void Process::start(ProcessHandle myHandle, ProcessManagerPtr owner)
		{
			//метод вызываеться один раз для каждого процесса
			//assert((m_currentStatus == k_processStatusNotStarted) && "попытка запустить уже запущенный процесс");

			m_handle = myHandle;
			m_owner = owner;
			m_currentStatus = k_processStatusRunning;

			LOGI("process [0x%x] started", this);
		}

		void Process::suspend()
		{
			if(m_currentStatus == k_processStatusRunning)
			{
				m_currentStatus = k_processStatusSuspended;
				LOGI("process [0x%x] suspended", this);
			}
		}
		
		void Process::resume()
		{
			if(m_currentStatus == k_processStatusSuspended)
			{
				m_currentStatus = k_processStatusRunning;
				LOGI("process [0x%x] resumed", this);
			}
		}

		void Process::terminate()
		{
			m_currentStatus = k_processStatusKilled;
			LOGI("process [0x%x] terminated", this);
		}

		void Process::attachNext(ProcessPtr nextProcess)
		{
			m_next = nextProcess;			
		}
		
		ProcessPtr Process::getNext() const
		{
			return m_next;
		}

		/*********************************************************************************
		//      ProcessManager class implementation		
		**********************************************************************************/
		const MILLISECONDS ProcessManager::kTimeNoLimit = -1;

		
		void ProcessManager::updateProcesses(MILLISECONDS deltaTime, MILLISECONDS timeLimit)
		{
			ProcessList killedProcesses;
			
			ProcessQueue& currentQueue = m_processQueue[m_currentQueue];
			ProcessQueue& otherQueue = m_processQueue[1 - m_currentQueue];

			if(timeLimit != kTimeNoLimit)
			{
				if(currentQueue.size() == 0)
				{
					m_processTimeLimit = timeLimit;
				}else
				{
					m_processTimeLimit = currentQueue.size() <= timeLimit ? (MILLISECONDS)(timeLimit * 1.0 / currentQueue.size()) : 1;
				}			 				
			}else
			{
				m_processTimeLimit = kTimeNoLimit;
			}
			
			double lastTime = m_timer->now();
			
			while(!currentQueue.empty())
			{
				ProcessHandle handle = currentQueue.front();
				currentQueue.pop();

				ProcessPtr process = m_activeProcesses[handle];

				if(process->getStatus() == k_processStatusKilled)
				{
					killedProcesses.push_back(handle);
				}else
				{
					otherQueue.push(handle);					
				}

				if(process->getStatus() == k_processStatusRunning)
				{
					process->update(deltaTime);
				}

				if(timeLimit != kTimeNoLimit)
				{
					double currentTime = m_timer->now();
					MILLISECONDS ellapsedTime = (MILLISECONDS)(currentTime - lastTime) * 1.0e3;

					if(ellapsedTime >= timeLimit)
						break;					
				}				
			}//while(!currentQueue.empty())
			
			

			for(ProcessList::iterator it = killedProcesses.begin();
				it != killedProcesses.end(); ++it)
			{
				ProcessPtr nextProcess = m_activeProcesses[*it]->getNext();
				if(nextProcess != (long)0)
				{
					attachProcess(nextProcess);
				}
				m_activeProcesses.erase(*it);
			}
			killedProcesses.clear();

			for(ProcessMap::iterator it = m_newProcesses.begin();
				it != m_newProcesses.end(); ++it)
			{
				ProcessHandle handle = it->first;
				ProcessPtr process = it->second;
				m_activeProcesses.insert(std::make_pair(handle, process));
                
				process->start(handle, this);
				currentQueue.push(handle);
			}
			m_newProcesses.clear();

			if(currentQueue.empty())
			{
				m_currentQueue = 1 - m_currentQueue;
			}
		}
			
		ProcessHandle ProcessManager::attachProcess(ProcessPtr process)
		{
			ProcessHandle handle = getNextHandle();
			m_newProcesses.insert(std::make_pair(handle, process));

			return handle;
		}

		ProcessPtr ProcessManager::getProcess(const ProcessHandle& handle) const
		{
			
			ProcessMap::const_iterator found_it = m_activeProcesses.find(handle);
			if(found_it != m_activeProcesses.end())
			{
				return found_it->second;
			}

			found_it  = m_newProcesses.find(handle);
			if(found_it != m_activeProcesses.end())
			{
				return found_it->second;
			}
			
			return ProcessPtr();
		}

		ProcessPtr ProcessManager::findActiveProcess(const ProcessHandle& handle)
		{
			ProcessMap::iterator found_it = m_activeProcesses.find(handle);
			if(found_it != m_activeProcesses.end())
			{
				return found_it->second;
			}

			return ProcessPtr();
		}

		void ProcessManager::suspendProcess(const ProcessHandle& handle)
		{
			ProcessPtr process = findActiveProcess(handle);
			if(process != (long)0)
			{
				process->suspend();
			}
		}

		void ProcessManager::resumeProcess(const ProcessHandle& handle)
		{
			ProcessPtr process = findActiveProcess(handle);
			if(process != (long)0)
			{
				process->resume();
			}
		}

		void ProcessManager::terminateProcess(const ProcessHandle& handle)
		{
			ProcessPtr process = findActiveProcess(handle);
			if(process != (long)0)
			{
				process->terminate();
			}
		}

		ProcessStatus ProcessManager::getProcessStatus(const ProcessHandle& handle) const
		{
			ProcessPtr process = getProcess(handle);
			if(process != (long)0)
			{
				process->getStatus();
			}

			//TODO: предусмотреть стратегию обработки ошибки
			//в случае если кто-то пытаеться запросить статус неизвестного процесса
			//выбросить пользовательское исключение если программа запущена в режиме отладки
			//произвести запись в лог если программа работает в пользовательском режиме

			return k_processStatusUnknown;
		}

		void ProcessManager::suspendAllProcesses()
		{
			for(ProcessMap::iterator it = m_activeProcesses.begin();
				it != m_activeProcesses.end(); ++it)
			{
				it->second->suspend();
			}
		}

		void ProcessManager::resumeAllProcesses()
		{
			for(ProcessMap::iterator it = m_activeProcesses.begin();
				it != m_activeProcesses.end(); ++it)
			{
				it->second->resume();
			}
		}

		void ProcessManager::terminateAllProcesses()
		{
			for(ProcessMap::iterator it = m_activeProcesses.begin();
				it != m_activeProcesses.end(); ++it)
			{
				it->second->terminate();
			}
		}

		void ProcessManager::dettachAllProcesses()
		{
			m_activeProcesses.clear();
			m_newProcesses.clear();
			
			while(!m_processQueue[0].empty())
			{
				m_processQueue[0].pop();
			}

			while(!m_processQueue[1].empty())
			{
				m_processQueue[1].pop();
			}
		}
		
        ProcessHandle ProcessManager::s_nextHandle = 0;

		ProcessHandle ProcessManager::getNextHandle()
		{
			return ++s_nextHandle;
		}	
}//namespace pegas
