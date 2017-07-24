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
#include "include/avsauthenticator.h"

using namespace Network::HTTP;

AVSAuthenticator::AVSAuthenticator()
    : m_code_grant("") ,
      m_refresh_token("") ,
      m_access_token("") ,
      m_client_id(""),
      m_client_secret_id(""),
      m_client_redirect_uri(""),
      m_product_id("")
{}

AVSAuthenticator::AVSAuthenticator( std::string client_id , std::string client_secret_id , std::string client_redirect_uri , std::string product_id )
    : m_code_grant("") ,
      m_refresh_token("") ,
      m_access_token("") ,
      m_client_id( client_id ),
      m_client_secret_id( client_secret_id),
      m_client_redirect_uri( client_redirect_uri ) ,
      m_product_id( product_id )
{}

void AVSAuthenticator::setProductID( string product_id )
{
    m_product_id = product_id;
}

void AVSAuthenticator::setClientID(string client_id)
{
    m_client_id = client_id;
}

void AVSAuthenticator::setClientSecretID(string client_secret_id)
{
    m_client_secret_id = client_secret_id;
}

void AVSAuthenticator::setRedirectURI(string redirect_uri)
{
    m_client_redirect_uri = redirect_uri;
}

void AVSAuthenticator::setRefreshToken( string refresh_token )
{
    m_refresh_token = refresh_token;
}

const std::string AVSAuthenticator::GenerateAmazonLoginPage(void)
{
   // Generate Amazon Login Page
   // https://www.amazon.com/ap/oa?client_id=${CLIENT_ID}&scope=${SCOPE}&scope_data=${SCOPE_DATA}&response_type=code&redirect_uri=https://localhost:3000/authresponse
   stringstream login_page;

   std::string login_page_final;
   std::string scope = "alexa:all";
   std::string scope_data = "{\"${SCOPE}\": {\"productID\": \"${PRODUCT_ID}\",\"productInstanceAttributes\": {\"deviceSerialNumber\": \"${DEVICE_SERIAL_NUMBER}\"}}}";

   replace( scope_data , "${SCOPE}" , scope );
   replace( scope_data , "${PRODUCT_ID}" , m_product_id );
   replace( scope_data , "${DEVICE_SERIAL_NUMBER}", "12345");

   login_page << "https://www.amazon.com/ap/oa?";
   login_page << "client_id=" << m_client_id;
   login_page << "&scope=" << scope;
   login_page << "&scope_data=" << scope_data;
   login_page << "&response_type=code";
   login_page << "&redirect_uri=" << m_client_redirect_uri;
   login_page << '\0';

   login_page_final = login_page.str();

   return login_page_final;
}

const std::string AVSAuthenticator::GenerateRefreshToken( const string code_grant ) throw(AuthenticatorState)
{
    if( m_code_grant.compare( code_grant ) == 0 ) // if equal
    {
        // code grant is a singlton ( 5min valid )
        throw AUTH_INVALID_CODE_GRANT;

    }else
    {
        // save code_grant
        m_code_grant = code_grant;

        // Generate Refresh Token by CODE GRANT
        // https://api.amazon.com/auth/o2/token
        // grant_type=authorization_code&code=${CODE}&client_id=${CLIENT_ID}&client_secret=${CLIENT_SECRET}&redirect_uri=${REDIRECT_URI}

        std::string refresh_token;
        stringstream req_post_data;
        std::string post_data;

        req_post_data << "grant_type=authorization_code";
        req_post_data << "&code=" << code_grant;
        req_post_data << "&client_id=" << m_client_id;
        req_post_data << "&client_secret=" << m_client_secret_id;
        req_post_data << "&redirect_uri=" << m_client_redirect_uri;
        req_post_data << '\0';

        post_data = req_post_data.str();
        url_encode( post_data );

        HTTPResponse * response = new HTTPResponse();
        HTTPRequest  * req = new HTTPRequest("https://api.amazon.com/auth/o2/token", response );

        req->addHeaderData("Content-Type: application/x-www-form-urlencoded");

        req->writeHeaderData();

        req->setPostData( post_data );

        req->doIt();

        req->clean();

        refresh_token = response_analyse_refresh_token( response );

        delete response;

        delete req;

        // save refresh_token
        m_refresh_token = refresh_token;

        // return token
        return refresh_token;
    }
}


const std::string AVSAuthenticator::GenerateAccessToken( string refresh_token ) throw(AuthenticatorState)
{
    // Generate AccessToken by REFRESH_TOKEN
    // https://api.amazon.com/auth/o2/token
    // grant_type=refresh_token&refresh_token=${REFRESH_TOKEN}&client_id=${CLIENT_ID}&client_secret=${CLIENT_SECRET}

    std::string access_token;
    std::string post_data;
    stringstream req_post_data;

    req_post_data << "grant_type=refresh_token";
    req_post_data << "&refresh_token=" << refresh_token;
    req_post_data << "&client_id=" << m_client_id;
    req_post_data << "&client_secret=" << m_client_secret_id;
    req_post_data << '\0';

    post_data = req_post_data.str();
    url_encode( post_data );

    HTTPResponse * response = new HTTPResponse();
    HTTPRequest * req = new HTTPRequest("https://api.amazon.com/auth/o2/token", response );

    req->addHeaderData("Content-Type: application/x-www-form-urlencoded");

    req->writeHeaderData();

    req->setPostData( post_data );

    req->doIt();

    req->clean();

    access_token = response_analyse_access_token( response );

    delete response;

    delete req;

    // save access_token
    m_access_token = access_token;

    // return token
    return access_token;
}

std::string AVSAuthenticator::response_analyse_refresh_token(HTTPResponse *response)
{
    AVSJson::json json_content = AVSJson::json::parse( response->getContent() );
    std::string error_description = "";
    std::string error = "";
    std::string refresh_token = "";

    try{

        refresh_token  = json_content["refresh_token"];

    }catch( std::exception e ) {

        // refresh_token string is null ( not found )
        error_description = json_content["error_description"];
        error             = json_content["error"];
    }

    if( !error_description.empty() )
    {
        // HTTP/1.1 400 Bad Request
        Logger::instance().warn("[CONFIG] CODE GRANT is INVALID");
        Logger::instance().warn("[CONFIG] remove CODE GRANT and restart AVS-PROTOTYPE");
        Logger::instance().error( error_description );
    }

    return refresh_token;
}

std::string AVSAuthenticator::response_analyse_access_token(HTTPResponse *response)
{
   AVSJson::json json_content = AVSJson::json::parse( response->getContent() );
   std::string error_description = "";
   std::string error             = "";
   std::string access_token      = "";

   try
   {

       access_token = json_content["access_token"];

   } catch ( std::exception e )
   {
       // access_token string is null ( not found )
       error_description = json_content["error_description"];
       error             = json_content["error"];
   }


   if( !error_description.empty() )
   {
      // HTTP/1.1 400 Bad Request
      Logger::instance().error( error_description );
   }

   return access_token;
}


void AVSAuthenticator::url_encode(string content)
{
    replace( content ,"|", "%7C");
    replace( content ,"=", "%3D");
    replace( content ,"&", "%26");
}

void AVSAuthenticator::replace(string &str, string search, string replace)
{
    if( search.empty() )
           return;

    size_t start_pos = 0;

    while((start_pos = str.find( search , start_pos)) != std::string::npos) {
           str.replace(start_pos, search.length(),  replace );
           start_pos += replace.length();
    }
}

const std::string AVSAuthenticator::getLastCodeGrant(void) const
{
    return m_code_grant;
}

const std::string AVSAuthenticator::getLastAccessToken(void) const
{
    return m_access_token;
}

const std::string AVSAuthenticator::getRefreshToken(void) const
{
    return m_refresh_token;
}
