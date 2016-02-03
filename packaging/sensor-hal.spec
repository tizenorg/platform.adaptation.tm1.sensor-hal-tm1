Name:       sensor-hal
Summary:    Sensor HAL Plugins
Version:    1.0.0
Release:    0
Group:      System/Sensor HAL
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    sensor-hal.manifest

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
Sensor HAL Plugins

%package sensor-hal
Summary:    Sensor HAL Plugins
Group:      System/Sensor Framework
Requires:   %{name} = %{version}-%{release}

%description sensor-hal
Sensor HAL Plugins

%prep
%setup -q
cp %{SOURCE1} .

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

%post -n sensor-hal -p /sbin/ldconfig

%postun -n sensor-hal -p /sbin/ldconfig

%files -n sensor-hal
%manifest sensor-hal.manifest
%attr(0644,root,root)/usr/etc/sensors.xml
%{_libdir}/sensor-hal/libsensor-hal.so
