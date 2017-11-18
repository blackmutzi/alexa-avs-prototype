/*
   Copyright (C) 2017 - 2018 this.mutzii@gmail.com

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef AVSAUTHENTICATOR_H
#define AVSAUTHENTICATOR_H

#include "stdafx.h"
#include "logger.h"
#include "httprequest.h"
#include "json.h"

using namespace Network::HTTP;
using namespace AVSJson;

enum AuthenticatorState
{
    AUTH_INVALID_CODE_GRANT
};


class AVSAuthenticator
{
    public:
        AVSAuthenticator();
        AVSAuthenticator( std::string client_id , std::string client_secret_id , std::string client_redirect_uri , std::string product_id );
        ~AVSAuthenticator(){}

        void setClientID( std::string client_id );
        void setClientSecretID( std::string client_secret_id);
        void setRedirectURI( std::string redirect_uri );
        void setProductID( std::string product_id );
        void setRefreshToken( std::string refresh_token );

        /*!
         * GET Amazon Login Page
         *  |=> Login & get CODE GRANT
         * \brief GenerateAmazonLoginPage
         * \return
         */
        const std::string GenerateAmazonLoginPage(void);

        /*!
         * Generate RefreshToken from CODE GRANT
         * \brief GenerateRefreshToken
         * \param code_grant
         * \return
         */
        const std::string GenerateRefreshToken( const std::string code_grant ) throw (AuthenticatorState);

        /*!
         * Generate new AccessTokens from RefreshToken
         * \brief GenerateAccessToken
         * \param refresh_token
         * \return
         */
        const std::string GenerateAccessToken( std::string refresh_token ) throw(AuthenticatorState);

        const std::string getLastCodeGrant(void) const;
        const std::string getLastAccessToken(void) const;
        const std::string getRefreshToken(void) const;

    protected:

        std::string response_analyse_refresh_token( HTTPResponse * response );
        std::string response_analyse_access_token( HTTPResponse * response );
        void url_encode( std::string content );
        void replace( string & str , string search , string replace );

    private:

     std::string m_code_grant;
     std::string m_refresh_token;
     std::string m_access_token;

     std::string m_client_id;
     std::string m_client_secret_id;
     std::string m_client_redirect_uri;

     std::string m_product_id;

};

#endif // AVSAUTHENTICATOR_H
