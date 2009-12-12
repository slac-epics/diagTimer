#include <iostream>
#include <iomanip>
#include <iocsh.h>
#include <epicsExport.h>
#include <registryFunction.h>
#include "ContextTimerMax.h"


using	namespace		std;

vector<ContextTimerMax *>	ContextTimerMax::ms_Instances;


void
ContextTimerMax::NewDur( const t_HiResTime	&	aNewDur	)
{
	m_CumDur	+= aNewDur;
	m_Count++;

	if ( m_MaxDur < aNewDur )
	{
		// New Maximum duration
		m_MaxDur	= aNewDur;
	}
}


ContextTimerMax::~ContextTimerMax()
{
	vector<ContextTimerMax *>::iterator		it;

	it = find( ms_Instances.begin(), ms_Instances.end(), this );
	if ( it != ms_Instances.end() )
	{
		ms_Instances.erase( it );
	}
}


double	ContextTimerMax::GetDurAvg( ) const
{
	double	cumDurSec	= HiResTicksToSeconds( m_CumDur );
	return	cumDurSec / m_Count;
}


void	ContextTimerMax::DisableAllContextTimers( )
{
	for (	vector<ContextTimerMax *>::iterator it = ms_Instances.begin();
			it != ms_Instances.end();
			it++	)
	{
		ContextTimerMax		*	pContextTimerMax	= *it;
		if ( pContextTimerMax == NULL )
			continue;
		pContextTimerMax->Disable( );
	}
}


void	ContextTimerMax::EnableAllContextTimers( )
{
	for (	vector<ContextTimerMax *>::iterator it = ms_Instances.begin();
			it != ms_Instances.end();
			it++	)
	{
		ContextTimerMax		*	pContextTimerMax	= *it;
		if ( pContextTimerMax == NULL )
			continue;
		pContextTimerMax->Enable( );
	}
}


void	ContextTimerMax::ResetAllContextTimers( )
{
	for (	vector<ContextTimerMax *>::iterator it = ms_Instances.begin();
			it != ms_Instances.end();
			it++	)
	{
		ContextTimerMax		*	pContextTimerMax	= *it;
		if ( pContextTimerMax == NULL )
			continue;
		pContextTimerMax->Reset( );
	}
}


int	ContextTimerMax::ShowAllContextTimers( )
{
	bool	fVerbose	= false;
	for (	vector<ContextTimerMax *>::iterator it = ms_Instances.begin();
			it != ms_Instances.end();
			it++	)
	{
		ContextTimerMax		*	pContextTimerMax	= *it;
		if ( pContextTimerMax == NULL )
			continue;

		if ( pContextTimerMax->GetCount() == 0 )
		{
			if ( fVerbose )
			{
				cout	<<	setiosflags(ios::left) <<	setw(60)
						<<	pContextTimerMax->m_ContextName
						<<	setiosflags(ios::right)
						<<	": No Instances Tracked"	<<	endl;
			}
			continue;
		}

		double	cumDurSec	= HiResTicksToSeconds( pContextTimerMax->m_CumDur );
		double	maxDurSec	= HiResTicksToSeconds( pContextTimerMax->m_MaxDur );
		cout	<<	setiosflags(ios::left)	<<	setw(60)
				<<	pContextTimerMax->m_ContextName
				<<	setiosflags(ios::right)
				<<	": CUM="	<<	setw(6)	<<	cumDurSec
				<<	"sec, MAX="	<<	setw(5)	<<	maxDurSec
				<<	"sec, AVG="	<<	setw(4)	<<	pContextTimerMax->GetDurAvg()
				<<	"sec, Cnt="	<<	setw(6)	<<	pContextTimerMax->m_Count;
		cout	<<	endl;
	}
	return 0;
}


int	ContextTimerMax::ShowContextTimer( const string & contextName )
{
	bool	fVerbose	= false;
	for (	vector<ContextTimerMax *>::iterator it = ms_Instances.begin();
			it != ms_Instances.end();
			it++	)
	{
		ContextTimerMax		*	pContextTimerMax	= *it;
		if ( pContextTimerMax == NULL )
			continue;
		if ( pContextTimerMax->m_ContextName != contextName )
			continue;

		if ( pContextTimerMax->GetCount() == 0 )
		{
			if ( fVerbose )
			{
				cout	<<	setiosflags(ios::left) <<	setw(60)
						<<	pContextTimerMax->m_ContextName
						<<	setiosflags(ios::right)
						<<	": No Instances Tracked"	<<	endl;
			}
			continue;
		}

		double	cumDurSec	= HiResTicksToSeconds( pContextTimerMax->m_CumDur );
		double	maxDurSec	= HiResTicksToSeconds( pContextTimerMax->m_MaxDur );
		cout	<<	setiosflags(ios::left)	<<	setw(60)
				<<	pContextTimerMax->m_ContextName
				<<	setiosflags(ios::right)
				<<	": CUM="	<<	setw(6)	<<	cumDurSec
				<<	"sec, MAX="	<<	setw(5)	<<	maxDurSec
				<<	"sec, AVG="	<<	setw(4)	<<	pContextTimerMax->GetDurAvg()
				<<	"sec, Cnt="	<<	setw(6)	<<	pContextTimerMax->m_Count;
		cout	<<	endl;
	}
	return 0;
}


extern "C" int DisableAllContextTimers( )
{
	ContextTimerMax::DisableAllContextTimers( );
	return 0;
}


extern "C" int EnableAllContextTimers( )
{
	ContextTimerMax::EnableAllContextTimers( );
	return 0;
}


extern "C" int ResetAllContextTimers( )
{
	ContextTimerMax::ResetAllContextTimers( );
	return 0;
}


extern "C" int ShowAllContextTimers( )
{
	return ContextTimerMax::ShowAllContextTimers( );
}


extern "C" int ShowContextTimer( const char * pszTimerName )
{
	return ContextTimerMax::ShowContextTimer( pszTimerName );
}


/* Register DisableAllContextTimers */
static const iocshFuncDef   DisableAllContextTimersFuncDef	= { "DisableAllContextTimers", 0, NULL };
static void  DisableAllContextTimersCallFunc( const iocshArgBuf * args )
{
	DisableAllContextTimers( );
}
static void DisableAllContextTimersRegister(void)
{
	iocshRegister( &DisableAllContextTimersFuncDef, DisableAllContextTimersCallFunc );
}

epicsExportRegistrar( DisableAllContextTimersRegister );


/* Register EnableAllContextTimers */
static const iocshFuncDef   EnableAllContextTimersFuncDef	= { "EnableAllContextTimers", 0, NULL };
static void  EnableAllContextTimersCallFunc( const iocshArgBuf * args )
{
	EnableAllContextTimers( );
}
static void EnableAllContextTimersRegister(void)
{
	iocshRegister( &EnableAllContextTimersFuncDef, EnableAllContextTimersCallFunc );
}

epicsExportRegistrar( EnableAllContextTimersRegister );


/* Register ResetAllContextTimers */
static const iocshFuncDef   ResetAllContextTimersFuncDef	= { "ResetAllContextTimers", 0, NULL };
static void  ResetAllContextTimersCallFunc( const iocshArgBuf * args )
{
	ResetAllContextTimers( );
}
static void ResetAllContextTimersRegister(void)
{
	iocshRegister( &ResetAllContextTimersFuncDef, ResetAllContextTimersCallFunc );
}

epicsExportRegistrar( ResetAllContextTimersRegister );


/* Register ShowAllContextTimers */
static const iocshFuncDef   ShowAllContextTimersFuncDef	= { "ShowAllContextTimers", 0, NULL };
static void  ShowAllContextTimersCallFunc( const iocshArgBuf * args )
{
	ShowAllContextTimers( );
}
static void ShowAllContextTimersRegister(void)
{
	iocshRegister( &ShowAllContextTimersFuncDef, ShowAllContextTimersCallFunc );
}

epicsExportRegistrar( ShowAllContextTimersRegister );


/* Register ShowContextTimer */
static const iocshArg		ShowContextTimerArg0	= { "contextName", iocshArgString };
static const iocshArg	*	ShowContextTimerArgs[1]	= { &ShowContextTimerArg0 };
static const iocshFuncDef   ShowContextTimerFuncDef	= { "ShowContextTimer", 1, ShowContextTimerArgs };
static void  ShowContextTimerCallFunc( const iocshArgBuf * args )
{
	ShowContextTimer( args[0].sval );
}
static void ShowContextTimerRegister(void)
{
	iocshRegister( &ShowContextTimerFuncDef, ShowContextTimerCallFunc );
}

epicsExportRegistrar( ShowContextTimerRegister );

