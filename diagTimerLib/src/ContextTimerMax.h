#ifndef CONTEXT_TIMER_MAX_H
#define CONTEXT_TIMER_MAX_H

#include <string>
#include <vector>
#include "HiResTime.h"



/// Purpose: Support class for ContextTimer
///
///	Common use is implicitly via CONTEXT_TIMER() macro
///	in ContextTimer.h, but can be invoked explicitly as show below if desired.
///	
///	Ex.	Track max duration spent in the following software context
///		{
///			static	ContextTimerMax	contextTimerMax( name );
///			ContextTimer			contextTimer( contextTimerMax );
///			...
///		}
///
/// or as a general purpose interval timer
///		{
///			static	ContextTimerMax	timer1( name );
///			timer1.StartTimer( );
///				... run some code ...
///			timer1.StopTimer( );
///		}
///	
class ContextTimerMax
{
public:
    /// Constructors.
    ContextTimerMax( const char *	pszContextName	)
		:	m_ContextName(		pszContextName		),
			m_MaxDur(			0LL					),
			m_CumDur(			0LL					),
			m_RateStartTime(	0LL					),
			m_StartTime(		-1LL				),
			m_Count(			0					),
			m_Enabled(			true				)
	{
		if( pszContextName == NULL || *pszContextName == '\0' )
			m_ContextName = "NoName";

		printf( "Registering ContextTimerMax %s, %zu total\n", m_ContextName.c_str(), ms_Instances.size() );
		ms_Instances.push_back( this );
	}

	/// Destructor
	virtual ~ContextTimerMax();

	/// Reset the tracked counts and durations
	void		Reset( )
	{
		m_MaxDur	= 0LL;
		m_CumDur	= 0LL;
		m_Count		= 0;
		// Restart the clock for rate calculations
		m_RateStartTime	= GetCurTime( );
	}

 	/// Disable the tracking
	void		Disable( )
	{
		m_Enabled	= false;
	}

 	/// Enable the tracking
	void		Enable( )
	{
		m_Enabled	= true;
	}

 	/// Get the average duration
	double	GetDurAvg( ) const;

 	/// Get the average rate since last reset in counts/sec
	double	GetRate( ) const;

 	/// Get the current cumulative duration
	t_HiResTime	GetDurCum( ) const
	{
		return m_CumDur;
	}

 	/// Get the current max duration
	t_HiResTime	GetDurMax( ) const
	{
		return m_MaxDur;
	}

 	/// Get the number of times NewDur() was called
	unsigned int	GetCount( ) const
	{
		return m_Count;
	}

	/// Get the current HiRes Tick count
	static t_HiResTime 	GetCurTime( )
	{
		return	GetHiResTicks();
	}

 	/// Return true if enabled for tracking
	bool	IsEnabled( ) const
	{
		return m_Enabled;
	}

 	/// Update with a new duration
	void	NewDur( const t_HiResTime	& newDur );

	void	CancelTimer( )
	{
		m_StartTime	= -1LL;
	}

	void	StartTimer( )
	{
		m_StartTime	= GetCurTime( );
	}

	void	StopTimer( )
	{
		if ( m_StartTime >= 0 )
		{
			t_HiResTime	dur( GetCurTime( ) );
			dur	-= m_StartTime;
			NewDur( dur );
		}
	}

	// Public class functions

    /// Disable all the ContextTimerMax instances
	static void	DisableAllContextTimers( );

    /// Enable all the ContextTimerMax instances
	static void	EnableAllContextTimers( );

    /// Reset counts and durations for all ContextTimerMax instances
	static void	ResetAllContextTimers( );

	/// Show the current stats for all ContextTimerMax instances on stdout
	static int	ShowAllContextTimers( );

	/// Show the current stats for the specified ContextTimerMax instance on stdout
	static int	ShowContextTimer( const std::string & contextName );

private:
	// Copying of this class is prohibited:
    ContextTimerMax(const ContextTimerMax&);

	// Assignment to this class is prohibited:
    ContextTimerMax& operator=(const ContextTimerMax&);

	std::string				m_ContextName;
	t_HiResTime				m_MaxDur;
	t_HiResTime				m_CumDur;
	t_HiResTime				m_RateStartTime;
	t_HiResTime				m_StartTime;
	unsigned int			m_Count;
	bool					m_Enabled;

	// Private class variables
	static std::vector<ContextTimerMax *>	ms_Instances;
};

#endif // #ifndef CONTEXT_TIMER_MAX_H
