Summary: Magick IRC Services
Name: Magick
Version: 2.0
Release: 1
Copyright: GPL
Group: Daemons
Source: ftp.magick.tm/pub/Magick/beta/Magick-2.0b3.tar.gz
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
configure --prefix=/usr --sysconfdir=/etc
helper/build-ver .
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
make install RPM_OPT_FLAGS="$RPM_OPT_FLAGS"
rm -Rf /usr/info/magick

%files
%doc AUTHORS COPYING ChangeLog INSTALL KNOWN-BUGS NEWS README TODO
%doc docs/en/index.html docs/en/index-1.html docs/en/index-2.html
%doc docs/en/index-3.html docs/en/index-4.html docs/en/index-5.html
%doc docs/en/index-6.html
%config /etc/magick.ini

/usr/bin/magick
/usr/bin/magick_keygen
/usr/share/magick
/usr/man/man1/magick.1
/usr/man/man1/magick_keygen.1

