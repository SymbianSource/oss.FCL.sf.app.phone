# ==============================================================================
# Generated by qmake (2.01a) (Qt 4.5.2-tower) on: ti 29. syys 12:32:49 2009
# This file is generated by qmake and should not be modified by the
# user.
#  Name        : Makefile_0xE35130c6.mk
#  Part of     : csplugin
#  Description : This file is used to call necessary targets on wrapper makefile
#                during normal Symbian build process.
#  Version     : 
#
# ==============================================================================



MAKE = make

do_nothing :
	@rem do_nothing

MAKMAKE: create_temps pre_targetdeps

LIB: create_temps pre_targetdeps

BLD: create_temps pre_targetdeps

ifeq "$(PLATFORM)" "WINSCW"
CLEAN: extension_clean winscw_deployment_clean
else
CLEAN: extension_clean
endif

CLEANLIB: do_nothing

RESOURCE: do_nothing

FREEZE: do_nothing

SAVESPACE: do_nothing

RELEASABLES: do_nothing

ifeq "$(PLATFORM)" "WINSCW"
FINAL: finalize winscw_deployment
else
FINAL: finalize
endif

pre_targetdeps : r:\sf\app\phone\csplugin\Makefile
	-$(MAKE) -f "r:\sf\app\phone\csplugin\Makefile" pre_targetdeps

create_temps : r:\sf\app\phone\csplugin\Makefile
	-$(MAKE) -f "r:\sf\app\phone\csplugin\Makefile" create_temps

extension_clean : r:\sf\app\phone\csplugin\Makefile
	-$(MAKE) -f "r:\sf\app\phone\csplugin\Makefile" extension_clean

finalize : r:\sf\app\phone\csplugin\Makefile
	-$(MAKE) -f "r:\sf\app\phone\csplugin\Makefile" finalize

winscw_deployment_clean : r:\sf\app\phone\csplugin\Makefile
	-$(MAKE) -f "r:\sf\app\phone\csplugin\Makefile" winscw_deployment_clean

winscw_deployment : r:\sf\app\phone\csplugin\Makefile
	-$(MAKE) -f "r:\sf\app\phone\csplugin\Makefile" winscw_deployment

