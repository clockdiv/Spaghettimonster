{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 1,
			"revision" : 11,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 437.0, 192.0, 601.0, 816.0 ],
		"bglocked" : 0,
		"openinpresentation" : 1,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"comment" : "",
					"id" : "obj-1",
					"index" : 0,
					"maxclass" : "inlet",
					"numinlets" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 66.692927002906799, 29.697986781597137, 30.0, 30.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-36",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 56.003406524658203, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 37.906722068786621, 61.287240960597984, 12.151925939917533, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 2,
					"fontname" : "Arial",
					"id" : "obj-35",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 138.26606422662735, 125.112103521823883, 140.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 89.832744843959745, 75.666655109226696, 38.0, 20.0 ],
					"text" : "invert"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 71.891615524888039, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 119.213915172815291, 80.269805387556545, 9.512902159094779, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 135.444451525807381, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 78.647027417719301, 65.252352626025669, 10.638124242424965, 19.436904840171337 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 40.115197524428368, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 83.453995333909972, 80.269805387556545, 9.866962332129447, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 119.556242525577545, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 78.643587673008426, 61.287240960597984, 50.083229658901644, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 103.66803352534771, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 33.162838566899268, 24.574308612346641, 10.638124242424965, 40.506631791591644 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-14",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 87.779824525117874, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 32.93786896020174, 20.757288568615905, 112.112351968586381, 8.521357856690884 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-9",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 102.0, 345.087605297565517, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 127.050220928788121, 75.666655109226696, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-4",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 170.0, 285.5, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 127.050220928788121, 56.684090682268135, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 66.692927002906799, 316.099099099636078, 29.5, 22.0 ],
					"text" : "!= 1"
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-28",
					"lockeddragscroll" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 157.14870810508728, 390.134584954977072, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 77.166655109226696, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-18",
					"lockeddragscroll" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 157.14870810508728, 348.08760529756546, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 58.184090682268135, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-24",
					"lockeddragscroll" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 157.14870810508728, 97.822609522342674, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 16.329185405605315, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 66.692927002906799, 240.0, 42.0, 22.0 ],
					"text" : "> 0. 1."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "live.dial",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 128.638339787721634, 174.563536733388901, 41.0, 48.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 44.051114709973319, 39.8637023577094, 41.0, 48.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.5 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "live.dial[1]",
							"parameter_mmax" : 1.0,
							"parameter_shortname" : "threshold",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"varname" : "live.dial[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "live.dial",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 66.692927002906799, 174.563536733388901, 41.0, 48.0 ],
					"presentation" : 1,
					"presentation_rect" : [ -2.060612881481632, 0.957287886142723, 41.0, 48.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_longname" : "live.dial",
							"parameter_mmax" : 1.0,
							"parameter_shortname" : "input",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"varname" : "live.dial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 2 ],
					"order" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"order" : 1,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"order" : 1,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"order" : 0,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 2 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 1 ],
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 2 ],
					"source" : [ "obj-9", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-18::obj-32::obj-15" : [ "Unmap[1]", "unmap", 0 ],
			"obj-18::obj-32::obj-19" : [ "Map[1]", "Map", 0 ],
			"obj-24::obj-32::obj-15" : [ "Unmap[2]", "unmap", 0 ],
			"obj-24::obj-32::obj-19" : [ "Map[2]", "Map", 0 ],
			"obj-28::obj-32::obj-15" : [ "Unmap[22]", "unmap", 0 ],
			"obj-28::obj-32::obj-19" : [ "Map[23]", "Map", 0 ],
			"obj-6" : [ "live.dial", "input", 0 ],
			"obj-8" : [ "live.dial[1]", "threshold", 0 ],
			"parameterbanks" : 			{

			}
,
			"parameter_overrides" : 			{
				"obj-18::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[1]"
				}
,
				"obj-18::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[1]"
				}
,
				"obj-24::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[2]"
				}
,
				"obj-24::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[2]"
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "M4L.IO.Remote.maxpat",
				"bootpath" : "~/Desktop/maxmsp-rumgeeiere",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "M4L.IO.MapFunction.maxpat",
				"bootpath" : "~/Desktop/maxmsp-rumgeeiere",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "M4L.IO.MapButton.maxpat",
				"bootpath" : "~/Desktop/maxmsp-rumgeeiere",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
 ],
		"autosave" : 0,
		"styles" : [ 			{
				"name" : "AudioStatus_Menu",
				"default" : 				{
					"bgfillcolor" : 					{
						"type" : "color",
						"color" : [ 0.294118, 0.313726, 0.337255, 1 ],
						"color1" : [ 0.454902, 0.462745, 0.482353, 0 ],
						"color2" : [ 0.290196, 0.309804, 0.301961, 1 ],
						"angle" : 270,
						"proportion" : 0.39,
						"autogradient" : 0
					}

				}
,
				"parentstyle" : "",
				"multi" : 0
			}
 ]
	}

}
