Name:       status
Summary:    status library
Version:    0.0.1
Release:    2
Group:      TBD
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(aul)
BuildRequires: cmake
Requires(post): /sbin/ldconfig
requires(postun): /sbin/ldconfig
%description
Status library.

%prep
%setup -q

%package devel
Summary:    Status library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Status library (devel).

%build
export LDFLAGS+="-Wl,--rpath=%{_prefix}/lib -Wl,--as-needed"
LDFLAGS="$LDFLAGS" cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make %{?jobs:-j%jobs}

%install
%make_install

mkdir -p %{buildroot}/usr/share/license
cp -f LICENSE %{buildroot}/usr/share/license/%{name}


%post
init_vconf()
{
    vconftool set -t int memory/private/libstatus/music/state 0 -i -g 5000
    vconftool set -t int memory/private/libstatus/radio/state 0 -i -g 5000
    vconftool set -t int memory/private/libstatus/vr/state 0 -i -g 5000
    vconftool set -t int memory/private/libstatus/music/pid -1 -i -g 5000
    vconftool set -t int memory/private/libstatus/radio/pid -1 -i -g 5000
    vconftool set -t int memory/private/libstatus/vr/pid -1 -i -g 5000
    vconftool set -t string memory/private/libstatus/message "" -i -g 5000
}

/sbin/ldconfig
init_vconf

%postun -p /sbin/ldconfig

%files
%manifest status.manifest
%defattr(-,root,root,-)
%{_libdir}/*.so*
/usr/share/license/%{name}

%files devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/*.h
%{_libdir}/pkgconfig/*.pc

