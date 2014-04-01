#pragma once
#ifndef DOCUMENTOBSERVER_H_
#define DOCUMENTOBSERVER_H_
class DocumentObserver
{
public:
	DocumentObserver(void);
	virtual ~DocumentObserver(void);
	virtual void Observe() = 0;
};
#endif

