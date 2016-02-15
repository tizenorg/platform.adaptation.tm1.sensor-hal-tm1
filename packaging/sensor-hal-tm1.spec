Name:       sensor-hal-tm1
Summary:    TM1 Sensor HAL
Version:    1.0.1
Release:    0
Group:      Service/Sensor
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

%if "%{?profile}" == "mobile"
ExcludeArch: aarch64 %ix86 x86_64
%else
ExcludeArch: %{arm} aarch64 %ix86 x86_64
%endif

BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gio-2.0)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  sensor-hal-devel

%description
TM1 Sensor HAL

%prep
%setup -q

%build
export CXXFLAGS+=" -Wextra -Wcast-align -Wcast-qual -Wshadow -Wwrite-strings -Wswitch-default"
export CXXFLAGS+=" -Wnon-virtual-dtor -Wno-c++0x-compat -Wno-unused-parameter -Wno-empty-body"
export CXXFLAGS+=" -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-strict-aliasing"
export CXXFLAGS+=" -fno-unroll-loops -fsigned-char -fstrict-overflow"
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
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
%{_libdir}/sensor/*.so
