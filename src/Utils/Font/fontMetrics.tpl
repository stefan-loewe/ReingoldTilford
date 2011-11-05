<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <title>
     $FONT_DEFINITION
    </title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <script type="text/javascript">
      /* <![CDATA[ */
      var charCodeToWidth = new Array();

      function getWidths()
      {
        elems = document.getElementsByTagName("span");

        for(var i = 0; i < elems.length; i++)
            charCodeToWidth[i] = elems[i].offsetWidth;

        scriptElement = document.createElement('script');

        scriptElement.src = $BACKEND + "?fontDefinitions=" + charCodeToWidth.join() + "&fontFamily=" + getStyle(elems[0], "font-family") + "&fontSize=" + parseFloat(getStyle(elems[0], "font-size"));

        document.getElementById("machineGeneratedCode").appendChild(scriptElement);
      }

      function getStyle(oElm, strCssRule)
      {
      	strValue = '';

        if(document.defaultView && document.defaultView.getComputedStyle)
      		strValue = document.defaultView.getComputedStyle(oElm, "").getPropertyValue(strCssRule);

      	else if(oElm.currentStyle)
        {
      		strCssRule = strCssRule.replace(/\-(\w)/g,  function (strMatch, p1)
                                                      {
      			                                             return p1.toUpperCase();
      		                                            });

          strValue = oElm.currentStyle[strCssRule];
      	}

      	return strValue;
      }
      /* ]]> */
    </script>
    <style type="text/css">
      body #machineGeneratedCode
      {
        visibility: hidden;
      }
      $STYLE
    </style>
  </head>

  <body>
    <div id="machineGeneratedCode">
        $CONTENT
        <script type="text/javascript">
          getWidths();
        </script>
    </div>
  </body>
</html>