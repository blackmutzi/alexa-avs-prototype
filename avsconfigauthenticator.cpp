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
#include "include/avsconfigauthenticator.h"

AVSConfigAuthenticator::AVSConfigAuthenticator( const std::string & config_file )
    : AVSAuthenticator() ,
      m_config_file( config_file )
{}

void AVSConfigAuthenticator::readConfig(void)
{
    std::string product_id;
    std::string client_id;
    std::string client_secret;
    std::string redirect_uri;
    std::string code_grant;
    std::string refresh_token;

    AVSJson::json json_reader;
    JsonFileLoader loader( m_config_file );
    loader.load();

    try{

    json_reader = AVSJson::json::parse(  loader.getJsonObject() );

    product_id     = json_reader["product_id"];
    client_id      = json_reader["client_id"];
    client_secret  = json_reader["client_secret"];
    redirect_uri   = json_reader["redirect_uri"];
    code_grant     = json_reader["code_grant"];
    refresh_token  = json_reader["refresh_token"];

    }catch( std::exception e )
    {
        Logger::instance().error("[CONFIG] corrupted config ");
    }

    if( product_id.empty() )
            onEventErrorSet("product_id");

    if( client_id.empty() )
            onEventErrorSet("client_id");

    if( client_secret.empty())
            onEventErrorSet("client_secret");

    if( redirect_uri.empty() )
            onEventErrorSet("redirect_uri");


    Logger::instance().info("[CONFIG] -- READ PRODUCT ID -- ");
    setProductID( product_id );

    Logger::instance().info("[CONFIG] -- READ CLIENT ID -- ");
    setClientID( client_id );

    Logger::instance().info("[CONFIG] -- READ CLIENT SECRET -- ");
    setClientSecretID( client_secret );

    Logger::instance().info("[CONFIG] -- READ REDIRECT URI -- ");
    setRedirectURI( redirect_uri );

    if( !refresh_token.empty() )
    {
        Logger::instance().info("[CONFIG] -- READ REFRESH TOKEN --");
        setRefreshToken( refresh_token );
        return;
    }

    if( code_grant.empty() )
    {
        Logger::instance().warn("[CONFIG] !! CODE GRANT not found !! ");
        Logger::instance().info("[CONFIG] Hello AVS User, go to this Amazon Page and Login.");
        Logger::instance().info(" ---- ");
        Logger::instance().info( GenerateAmazonLoginPage() );
        Logger::instance().info(" ---- ");
        Logger::instance().info("[CONFIG] And COPY the CODE GRANT into the CONFIG. Default ./config/config.json ");
        Logger::instance().info("[CONFIG] And RESTART AVS. ");

        exit(0);

    } else {

        Logger::instance().info("[CONFIG] !! CODE GRANT found !! ");
        Logger::instance().info("[CONFIG] Generate REFRESH TOKEN ");

        GenerateRefreshToken( code_grant );
        json_reader["refresh_token"] = getRefreshToken();

        ofstream config_file_stream( m_config_file );
        config_file_stream << json_reader;
        config_file_stream.close();

        Logger::instance().info("[CONFIG] CONFIG SAVED ");
    }
}

void AVSConfigAuthenticator::onEventErrorSet(const string name)
{
    stringstream err_stream;
    err_stream << "[CONFIG] " << name << " is in config.json empty!! " << std::endl;
    Logger::instance().error( err_stream.str() );
}



