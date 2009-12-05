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
class ContextTimerMax
{
public:
    /// Constructors.
    ContextTimerMax(	const char *	pszDurName	)
		:	m_pszDurName(		pszDurName			),
			m_MaxDur(			0					),
			m_CumDur(			0					),
			m_Count(			0					),
			m_Enabled(			true				)
	{
		if( m_pszDurName == NULL || *m_pszDurName == '\0' )
			m_pszDurName = "NoName";

		ms_Instances.push_back( this );
	}

	/// Destructor
	virtual ~ContextTimerMax();

	/// Reset the tracked counts and durations
	void		Reset( )
	{
		m_MaxDur	= 0;
		m_CumDur	= 0;
		m_Count		= 0;
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

 	/// Return true if enabled for tracking
	bool	IsEnabled( ) const
	{
		return m_Enabled;
	}

 	/// Update with a new duration
	void	NewDur( const t_HiResTime	& newDur );

	// Public class functions

    /// Disable all the ContextTimerMax instances
	static void	DisableAllContextTimer( );

    /// Enable all the ContextTimerMax instances
	static void	EnableAllContextTimer( );

    /// Reset counts and durations for all ContextTimerMax instances
	static void	ResetAllContextTimer( );

	/// Show the current stats for all ContextTimerMax instances on stdout
	static int	ShowAllContextTimer( );

private:
	// Copying of this class is prohibited:
    ContextTimerMax(const ContextTimerMax&);

	// Assignment to this class is prohibited:
    ContextTimerMax& operator=(const ContextTimerMax&);

	const char			*	m_pszDurName;
	t_HiResTime				m_MaxDur;
	t_HiResTime				m_CumDur;
	unsigned int			m_Count;
	bool					m_Enabled;

	// Private class variables
	static std::vector<ContextTimerMax *>	ms_Instances;
};

#endif // #ifndef CONTEXT_TIMER_MAX_H
