/* soapClient.cpp
   Generated by gSOAP 2.8.22 from wth.h

Copyright(C) 2000-2015, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapClient.cpp ver 2.8.22 2015-07-27 01:55:27 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__HelloWorld(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__HelloWorld *ns1__HelloWorld, _ns1__HelloWorldResponse &ns1__HelloWorldResponse)
{	struct __ns1__HelloWorld soap_tmp___ns1__HelloWorld;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://202.20.2.96/ws_material/ws_material.asmx";
	if (soap_action == NULL)
		soap_action = "http://tempuri.org/HelloWorld";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__HelloWorld.ns1__HelloWorld = ns1__HelloWorld;
	soap_serializeheader(soap);
	soap_serialize___ns1__HelloWorld(soap, &soap_tmp___ns1__HelloWorld);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__HelloWorld(soap, &soap_tmp___ns1__HelloWorld, "-ns1:HelloWorld", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__HelloWorld(soap, &soap_tmp___ns1__HelloWorld, "-ns1:HelloWorld", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__HelloWorldResponse)
		return soap_closesock(soap);
	ns1__HelloWorldResponse.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__HelloWorldResponse.soap_get(soap, "ns1:HelloWorldResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__add_USCOREmaterial(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__add_USCOREmaterial *ns1__add_USCOREmaterial, _ns1__add_USCOREmaterialResponse &ns1__add_USCOREmaterialResponse)
{	struct __ns1__add_USCOREmaterial soap_tmp___ns1__add_USCOREmaterial;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://202.20.2.96/ws_material/ws_material.asmx";
	if (soap_action == NULL)
		soap_action = "http://tempuri.org/add_material";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__add_USCOREmaterial.ns1__add_USCOREmaterial = ns1__add_USCOREmaterial;
	soap_serializeheader(soap);
	soap_serialize___ns1__add_USCOREmaterial(soap, &soap_tmp___ns1__add_USCOREmaterial);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__add_USCOREmaterial(soap, &soap_tmp___ns1__add_USCOREmaterial, "-ns1:add_material", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__add_USCOREmaterial(soap, &soap_tmp___ns1__add_USCOREmaterial, "-ns1:add_material", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__add_USCOREmaterialResponse)
		return soap_closesock(soap);
	ns1__add_USCOREmaterialResponse.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__add_USCOREmaterialResponse.soap_get(soap, "ns1:add_materialResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__HelloWorld_(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__HelloWorld *ns1__HelloWorld, _ns1__HelloWorldResponse &ns1__HelloWorldResponse)
{	struct __ns1__HelloWorld_ soap_tmp___ns1__HelloWorld_;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://202.20.2.96/ws_material/ws_material.asmx";
	if (soap_action == NULL)
		soap_action = "http://tempuri.org/HelloWorld";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__HelloWorld_.ns1__HelloWorld = ns1__HelloWorld;
	soap_serializeheader(soap);
	soap_serialize___ns1__HelloWorld_(soap, &soap_tmp___ns1__HelloWorld_);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__HelloWorld_(soap, &soap_tmp___ns1__HelloWorld_, "-ns1:HelloWorld", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__HelloWorld_(soap, &soap_tmp___ns1__HelloWorld_, "-ns1:HelloWorld", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__HelloWorldResponse)
		return soap_closesock(soap);
	ns1__HelloWorldResponse.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__HelloWorldResponse.soap_get(soap, "ns1:HelloWorldResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__add_USCOREmaterial_(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__add_USCOREmaterial *ns1__add_USCOREmaterial, _ns1__add_USCOREmaterialResponse &ns1__add_USCOREmaterialResponse)
{	struct __ns1__add_USCOREmaterial_ soap_tmp___ns1__add_USCOREmaterial_;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://202.20.2.96/ws_material/ws_material.asmx";
	if (soap_action == NULL)
		soap_action = "http://tempuri.org/add_material";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__add_USCOREmaterial_.ns1__add_USCOREmaterial = ns1__add_USCOREmaterial;
	soap_serializeheader(soap);
	soap_serialize___ns1__add_USCOREmaterial_(soap, &soap_tmp___ns1__add_USCOREmaterial_);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__add_USCOREmaterial_(soap, &soap_tmp___ns1__add_USCOREmaterial_, "-ns1:add_material", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__add_USCOREmaterial_(soap, &soap_tmp___ns1__add_USCOREmaterial_, "-ns1:add_material", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__add_USCOREmaterialResponse)
		return soap_closesock(soap);
	ns1__add_USCOREmaterialResponse.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__add_USCOREmaterialResponse.soap_get(soap, "ns1:add_materialResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapClient.cpp */
