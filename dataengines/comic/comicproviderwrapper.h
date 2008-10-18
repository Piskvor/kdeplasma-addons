/*
 *   Copyright (C) 2008 Petri Damstén <damu@iki.fi>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef COMICPROVIDERWRAPPER_H
#define COMICPROVIDERWRAPPER_H

#include "comicprovider.h"

#include <KUrl>
#include <QImage>
#include <QByteArray>

namespace Kross {
    class Action;
}
namespace Plasma {
    class Package;
}
class ComicProviderKross;

class ImageWrapper : public QObject
{
        Q_OBJECT
        Q_PROPERTY( QImage image READ image )
        Q_PROPERTY( QByteArray rawData READ rawData WRITE setRawData )
    public:
        QImage image() const;
        void setImage( const QImage &image );
        QByteArray rawData() const;
        void setRawData( const QByteArray &rawData );

    private:
        QImage mImage;
};

class ComicProviderWrapper : public QObject
{
        Q_OBJECT
        Q_ENUMS( IdentifierType )
        Q_ENUMS( RequestType )
        Q_ENUMS( PositionType )
        Q_PROPERTY( QString firstStripDate READ firstStripDate WRITE setFirstStripDate )
        Q_PROPERTY( int firstStripNumber READ firstStripNumber WRITE setFirstStripNumber )
        Q_PROPERTY( QString comicAuthor READ comicAuthor WRITE setComicAuthor )
    public:
        enum PositionType {
            Left = 0,
            Top,
            Right,
            Bottom
        };
        enum RequestType {
            Page = 0,
            Image,
            User
        };
        enum IdentifierType {
            DateIdentifier   = ComicProvider::DateIdentifier,
            NumberIdentifier = ComicProvider::NumberIdentifier,
            StringIdentifier = ComicProvider::StringIdentifier
        };

        ComicProviderWrapper( ComicProviderKross *parent );
        ~ComicProviderWrapper();

        ComicProvider::IdentifierType identifierType();
        KUrl websiteUrl();
        QImage image();
        QString identifier();
        QString nextIdentifier();
        QString previousIdentifier();
        QString firstStripIdentifier();
        QString stripTitle();
        QString additionalText();
        void pageRetrieved( int id, const QByteArray &data );
        void pageError( int id, const QString &message );

        QString firstStripDate() const;
        void setFirstStripDate( const QString &date );
        int firstStripNumber() const;
        void setFirstStripNumber( int number );
        QString comicAuthor() const;
        void setComicAuthor( const QString &author );
        void setUseDefaultImageHandler( bool useDefaultImageHandler );
        bool useDefaultImageHandler() const;

        bool functionCalled() const;

    public slots:
        void finished() const;
        void error() const;

        QString requestedDate() const;
        int requestedNumber() const;
        QString requestedString() const;
        void requestPage( const QString &url, int id, const QVariantMap &infos = QVariantMap() );
        void addHeader( const QString &name, PositionType position = Top );

        void init();

    protected:
        QVariant callFunction( const QString &name, const QVariantList &args = QVariantList() );
        const QStringList& extensions() const;

    private:
        Kross::Action *mAction;
        ComicProviderKross *mProvider;
        QStringList mFunctions;
        bool mFuncFound;
        ImageWrapper mKrossImage;
        static QStringList mExtensions;
        Plasma::Package *mPackage;
};

#endif
