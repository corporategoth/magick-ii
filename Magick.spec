Summary: Magick IRC Services
Name: Magick
Version: 2.0
Release: 1
Copyright: GPL
Group: Applications/Communications
Source: ftp.goth.net/pub/magick.tm/Magick-2.0.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
Magick IRC Services are a program that is designed to interface
with an Internet Relay Chat (IRC) network, and offer its users,
operators and administrators services such as the ability to
register nicknames so they cannot be taken by anyone else, the
ability to register channels with pre-defined access lists and
kick lists so that channels may not be taken over, the ability
to leave offline messages for other users, and many network
control mechanisms to stop things like flooding, abusive users,
and many other things.

%prep

%setup

%build
./configure --prefix=$RPM_BUILD_ROOT/usr --sysconfdir=$RPM_BUILD_ROOT/etc --enable-quick --disable-hostlock --enable-convert
helper/build-ver .
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr
make install RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog* FAQ INSTALL KNOWN-BUGS NEWS README TODO rtfm
%doc docs/en docs/rfcs
%config /etc/magick.ini

/usr/bin/magick
/usr/bin/magick_keygen
/usr/bin/magick_stampkeys
/usr/share/magick
/usr/man
