#ifndef CONTEXT_TIMER_H
#define CONTEXT_TIMER_H

#include "ContextTimerMax.h"

#define	ENABLE_CONTEXT_TIMER

#if defined(ENABLE_CONTEXT_TIMER)
#define	CONTEXT_TIMER(		contextId			)					\
	static	ContextTimerMax	contextTimerMax(	contextId		);	\
	ContextTimer			contextTimer(		contextTimerMax		)
#else
#define	CONTEXT_TIMER(		contextId		)	do { } while ( 0 )
#endif

///	Diagnostic support  for measuring execution time in a context
///
class ContextTimer
{
public:
    /// Constructor.
    ContextTimer(	ContextTimerMax	&	rDurMax )
		:
			m_StartTime(	0		),
			m_rDurMax(		rDurMax	)
	{
		if ( m_rDurMax.IsEnabled() )
		{
			m_StartTime	= GetCurTime( );
		}
	}

	/// Destructor
	~ContextTimer()
	{
		if ( m_rDurMax.IsEnabled() )
		{
			t_HiResTime	dur( GetCurTime( ) );
			dur	-= m_StartTime;
			m_rDurMax.NewDur( dur );
		}
	}

 	/// Get the current duration
	t_HiResTime	GetDur( )
	{
		t_HiResTime	dur( GetCurTime( ) );
		dur	-= m_StartTime;
		return dur;
	}

	/// Get the current HiRes Tick count
	static t_HiResTime 	GetCurTime( )
	{
		return	GetHiResTicks();
	}

private:
	// Copying of this class is prohibited:
    ContextTimer(const ContextTimer&);

	// Assignment to this class is prohibited:
    ContextTimer& operator=(const ContextTimer&);

	t_HiResTime				m_StartTime;
	ContextTimerMax		&	m_rDurMax;
};

#endif // #ifndef CONTEXT_TIMER_H
