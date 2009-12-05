#include <iostream>
#include <iomanip>
//#include <algorithm>
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


void	ContextTimerMax::DisableAllContextTimer( )
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


void	ContextTimerMax::EnableAllContextTimer( )
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


void	ContextTimerMax::ResetAllContextTimer( )
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


int	ContextTimerMax::ShowAllContextTimer( )
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
				cout	<<	setiosflags(ios::left) <<	setw(60)	<<	pContextTimerMax->m_pszDurName
						<<	setiosflags(ios::right)
						<<	": No Instances Tracked"	<<	endl;
			}
			continue;
		}

		double	cumDurSec	= HiResTicksToSeconds( pContextTimerMax->m_CumDur );
		double	maxDurSec	= HiResTicksToSeconds( pContextTimerMax->m_MaxDur );
		cout	<<	setiosflags(ios::left)	<<	setw(60)<<	pContextTimerMax->m_pszDurName
				<<	setiosflags(ios::right)
				<<	": CUM="	<<	setw(6)	<<	cumDurSec
				<<	"sec, MAX="	<<	setw(5)	<<	maxDurSec
				<<	"sec, AVG="	<<	setw(4)	<<	pContextTimerMax->GetDurAvg()
				<<	"sec, Cnt="	<<	setw(6)	<<	pContextTimerMax->m_Count;
		cout	<<	endl;
	}
	return 0;
}


extern "C" int DisableAllContextTimer( )
{
	ContextTimerMax::DisableAllContextTimer( );
	return 0;
}


extern "C" int EnableAllContextTimer( )
{
	ContextTimerMax::EnableAllContextTimer( );
	return 0;
}


extern "C" int ResetAllContextTimer( )
{
	ContextTimerMax::ResetAllContextTimer( );
	return 0;
}


extern "C" int ShowAllContextTimer( )
{
	return ContextTimerMax::ShowAllContextTimer( );
}
