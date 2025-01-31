#pragma once

#include <stdint.h>
#include <cstdlib>

size_t GetGlobalDisplayListSize();

class DisplayList
{
public:
	DisplayList();
	~DisplayList();
	DisplayList(const DisplayList&) = delete;
	DisplayList(DisplayList&&) = delete;
	void operator=(const DisplayList&) = delete;
	void operator=(DisplayList&&) = delete;
	void Render() const;
	void Begin(size_t sizeOfDisplayList);
	void End();
	void Clear();
	inline bool IsEmpty() const { return !m_pDispList && m_displayListSize == 0; }
	inline uint32_t GetDisplayListSize() const { return m_displayListSize; }

private:
	void* m_pDispList = nullptr;
	uint32_t m_displayListSize = 0;
};

