Name:       sensor-plugins-tm1
Summary:    TM1 Sensor Plugins
Version:    1.0.0
Release:    0
Group:      System/Sensor Framework
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(libxml-2.0)

%define accel_state ON
%define gyro_state OFF
%define proxi_state ON
%define light_state OFF
%define geo_state OFF
%define pressure_state OFF
%define temperature_state OFF
%define ultraviolet_state OFF
%define rv_state OFF
%define bio_led_red_state OFF

%description
TM1 Sensor Plugins

%prep
%setup -q

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DACCEL=%{accel_state} \
-DGYRO=%{gyro_state} -DPROXI=%{proxi_state} -DLIGHT=%{light_state} \
-DGEO=%{geo_state} -DPRESSURE=%{pressure_state} -DTEMPERATURE=%{temperature_state} \
-DRV=%{rv_state} -DULTRAVIOLET=%{ultraviolet_state} \
-DBIO_LED_RED=%{bio_led_red_state} \
-DLIBDIR=%{_libdir} -DINCLUDEDIR=%{_includedir}

%build
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%manifest packaging/%{name}.manifest
%attr(0644,root,root)/usr/etc/sensors.xml
%{_libdir}/sensor/*.so
