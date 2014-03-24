Name:       status
Summary:    status library
Version:    0.0.1
Release:    1
Group:      TBD
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: 	status.manifest
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(aul)
BuildRequires: cmake
BuildRequires:  pkgconfig(libtzplatform-config)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
Status library.

%prep
%setup -q
cp %{SOURCE1001} .

%package devel
Summary:    Status library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Status library (devel).

%build
export LDFLAGS+="-Wl,--rpath=%{_libdir} -Wl,--as-needed"
%cmake .
make %{?jobs:-j%jobs}

%install
%make_install

mkdir -p %{buildroot}/usr/share/license
cp -f LICENSE %{buildroot}/usr/share/license/%{name}


%post
users_gid=$(getent group %TZ_SYS_USER_GROUP | cut -f3 -d':')
init_vconf()
{
    vconftool set -t int memory/private/libstatus/music/state 0 -i -g $users_gid
    vconftool set -t int memory/private/libstatus/radio/state 0 -i -g $users_gid
    vconftool set -t int memory/private/libstatus/vr/state 0 -i -g $users_gid
    vconftool set -t int memory/private/libstatus/music/pid -1 -i -g $users_gid
    vconftool set -t int memory/private/libstatus/radio/pid -1 -i -g $users_gid
    vconftool set -t int memory/private/libstatus/vr/pid -1 -i -g $users_gid
    vconftool set -t string memory/private/libstatus/message "" -i -g $users_gid
}

/sbin/ldconfig
init_vconf

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/*.so*
/usr/share/license/%{name}

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/%{name}/*.h
%{_libdir}/pkgconfig/*.pc

